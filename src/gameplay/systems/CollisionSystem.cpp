#include <iostream>
#include "../systems/CollisionSystem.h"
#include "../components/EntityTags.h"
#include "../components/StatComponent.h"
#include "../components/Hitbox.h"
#include "../components/MovementComponents.h"
#include "../components/CollisionType.h" 
#include "../../utils/VectorMath.h"

using namespace std;

CollisionSystem::CollisionSystem(entt::registry& registry)
	: registry(registry)
{
	collisionEvents.reserve(200);
}

void CollisionSystem::update(float dt)
{
	detectCollisions();
	for (const auto& event : getCollisionEvents())
	{
		resolvePhysicalOverlap(event.entity1, event.entity2);
	}
}

bool CollisionSystem::isIntersect(entt::entity e1, entt::entity e2) const
{
	Hitbox& hitbox1 = registry.get<Hitbox>(e1);
	Hitbox& hitbox2 = registry.get<Hitbox>(e2);

	Position& pos1 = registry.get<Position>(e1);
	Position& pos2 = registry.get<Position>(e2);

	// Calculate hitbox positions  
	float hitbox1PosX = hitbox1.offsetX + pos1.x;
	float hitbox1PosY = hitbox1.offsetY + pos1.y;
	float hitbox2PosX = hitbox2.offsetX + pos2.x;
	float hitbox2PosY = hitbox2.offsetY + pos2.y;

	// Check if the hitboxes are the same  
	if (hitbox1 == hitbox2)
	{
		return false;
	}

	if (hitbox1.type == HitboxType::Rectangle && hitbox2.type == HitboxType::Rectangle)
	{
		float left1 = hitbox1PosX;
		float right1 = left1 + hitbox1.width;
		float top1 = hitbox1PosY;
		float bottom1 = top1 + hitbox1.height;
		float left2 = hitbox2PosX;
		float right2 = left2 + hitbox2.width;
		float top2 = hitbox2PosY;
		float bottom2 = top2 + hitbox2.height;
		return !(left1 > right2 || right1 < left2 || top1 > bottom2 || bottom1 < top2);
	}

	else if (hitbox1.type == HitboxType::Circle && hitbox2.type == HitboxType::Circle)
	{
		float dx = hitbox1PosX - hitbox2PosX;
		float dy = hitbox1PosY - hitbox2PosY;
		float distanceSquared = dx * dx + dy * dy;
		float radiusSum = hitbox1.radius + hitbox2.radius;
		return distanceSquared <= (radiusSum * radiusSum);
	}

	// One is rectangle, the other is circle
	else
	{
		if (hitbox1.type == HitboxType::Rectangle)
		{
			float circleX = hitbox2PosX;
			float circleY = hitbox2PosY;
			float rectLeft = hitbox1PosX;
			float rectRight = rectLeft + hitbox1.width;
			float rectTop = hitbox1PosY;
			float rectBottom = rectTop + hitbox1.height;
			if (circleX <= rectLeft || circleX >= rectRight || circleY <= rectTop || circleY >= rectBottom)
			{
				return false; // Circle is outside the rectangle  
			}
			// Check distance from circle center to rectangle edges  
			float closestX = std::max(rectLeft, std::min(circleX, rectRight));
			float closestY = std::max(rectTop, std::min(circleY, rectBottom));
			float dx = closestX - circleX;
			float dy = closestY - circleY;
			return (dx * dx + dy * dy) <= (hitbox2.radius * hitbox2.radius);
		}

		else // hitbox1.type == HitboxType::Circle  
		{
			return isIntersect(e2, e1); // Swap the order  
		}
	}
}

void CollisionSystem::detectCollisions()
{
	collisionEvents.clear();
	SpatialHashGrid spatialHashGrid(50.0f);
	spatialHashGrid.makeGrid(registry);

	auto view = registry.view<Position, Hitbox>();
	for (auto [entity, position, hitbox] : view.each())
	{
		auto nearbyEntities = spatialHashGrid.queryNearby(position);

		for (auto otherEntity : nearbyEntities)
		{

			//Can wrap it in function
			//inline bool shouldProcessPair(entt::entity a, entt::entity b) {
			//	return static_cast<uint32_t>(a) < static_cast<uint32_t>(b);
			//}
			if (entity >= otherEntity)
			{
				continue; // Avoid double checking pairs and self collision
			}

			Hitbox& otherHitbox = registry.get<Hitbox>(otherEntity);
			if (isIntersect(entity, otherEntity))
			{
				CollisionType type1 = getCollisionType(entity);
				CollisionType type2 = getCollisionType(otherEntity);
				
				// Trigger collision event using dispatcher from registry context
				if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
					(*dispatcher)->trigger<CollisionEvent>(CollisionEvent{ entity, otherEntity, type1, type2 });
				}
				
				if (isSolid(type1, type2))
				{
					resolvePhysicalOverlap(entity, otherEntity);
				}
			}
		}
	}
}

void CollisionSystem::resolvePhysicalOverlap(entt::entity e1, entt::entity e2)
{
	auto type1 = getCollisionType(e1);
	auto type2 = getCollisionType(e2);

	if (type1 == CollisionType::None || type2 == CollisionType::None)
	{
		cerr << "Cannot resolve overlap for entities with no collision type." << endl;
		return; // Cannot resolve overlap if one of the entities has no collision type
	}

	if (!registry.all_of<Hitbox, Position, RepelResistance>(e1) || !registry.all_of<Hitbox, Position, RepelResistance>(e2))
	{
		throw std::runtime_error("Both entities must have Hitbox, Position, and RepelResistance components to resolve overlap.");
	}

	Hitbox& hitbox1 = registry.get<Hitbox>(e1);
	Hitbox& hitbox2 = registry.get<Hitbox>(e2);

	if (hitbox1.type == hitbox2.type)
	{
		if (hitbox1.type == HitboxType::Rectangle)
		{
			resolveRR(e1, e2);
		}
		else if (hitbox1.type == HitboxType::Circle)
		{
			resolveCC(e1, e2);
		}
		else
		{
			cerr << "Unsupported hitbox type for resolving overlap." << endl;
		}
	}
	else
	{
		cerr << "Cannot resolve overlap between different hitbox types." << endl;
	}
}

void CollisionSystem::resolveRR(entt::entity e1, entt::entity e2)
{
	Hitbox& hitbox = registry.get<Hitbox>(e1);
	Hitbox& otherHitbox = registry.get<Hitbox>(e2);

	if (hitbox.type != HitboxType::Rectangle || otherHitbox.type != HitboxType::Rectangle)
	{
		cerr << "Currently only rectangle hitboxes are supported for resolving overlap." << endl;
		return;
	};

	float resistance1 = registry.get<RepelResistance>(e1).value;
	float resistance2 = registry.get<RepelResistance>(e2).value;
	float totalResistance = resistance1 + resistance2;

	//0.01f is added to ensure two entities will not be completely stuck together
	float recoil1 = (resistance2 / totalResistance + 0.01f);
	float recoil2 = (resistance1 / totalResistance + 0.01f);

	Position& pos = registry.get<Position>(e1);
	Position& otherPos = registry.get<Position>(e2);

	float centerX1 = pos.x + hitbox.offsetX + hitbox.width / 2.0f;
	float centerY1 = pos.y + hitbox.offsetY + hitbox.height / 2.0f;

	float centerX2 = otherPos.x + otherHitbox.offsetX + otherHitbox.width / 2.0f;
	float centerY2 = otherPos.y + otherHitbox.offsetY + otherHitbox.height / 2.0f;

	float deltaX = centerX2 - centerX1;
	float deltaY = centerY2 - centerY1;

	float overlapX = (hitbox.width + otherHitbox.width) / 2.0f - std::abs(deltaX);
	float overlapY = (hitbox.height + otherHitbox.height) / 2.0f - std::abs(deltaY);

	if (overlapX < 0 || overlapY < 0)
	{
		cerr << "No overlap detected, cannot resolve." << endl;
		return; // No overlap to resolve
	}

	if (overlapX < overlapY)
	{
		// Resolve along X axis
		float moveAmount1 = recoil1 * overlapX;
		float moveAmount2 = recoil2 * overlapX;

		if (deltaX > 0)
		{
			pos.x -= moveAmount1; // Move e1 left
			otherPos.x += moveAmount2; // Move e2 right
		}
		else
		{
			pos.x += moveAmount1; // Move e1 right
			otherPos.x -= moveAmount2; // Move e2 left
		}
	}
	else
	{
		// Resolve along Y axis
		float moveAmount1 = recoil1 * overlapY;
		float moveAmount2 = recoil2 * overlapY;

		if (deltaY > 0)
		{
			pos.y -= moveAmount1; // Move e1 up
			otherPos.y += moveAmount2; // Move e2 down
		}
		else
		{
			pos.y += moveAmount1; // Move e1 down
			otherPos.y -= moveAmount2; // Move e2 up
		}
	}
}

void CollisionSystem::resolveCC(entt::entity e1, entt::entity e2)
{
	Hitbox& hitbox = registry.get<Hitbox>(e1);
	Hitbox& otherHitbox = registry.get<Hitbox>(e2);

	Position& pos = registry.get<Position>(e1);
	Position& otherPos = registry.get<Position>(e2);

	float centerX1 = pos.x + hitbox.offsetX + hitbox.radius;
	float centerY1 = pos.y + hitbox.offsetY + hitbox.radius;

	float centerX2 = otherPos.x + otherHitbox.offsetX + otherHitbox.radius;
	float centerY2 = otherPos.y + otherHitbox.offsetY + otherHitbox.radius;

	float deltaX = centerX2 - centerX1;
	float deltaY = centerY2 - centerY1;

	float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
	if (distance < hitbox.radius + otherHitbox.radius)
	{
		float overlap = hitbox.radius + otherHitbox.radius - distance;

		float resistance1 = registry.get<RepelResistance>(e1).value;
		float resistance2 = registry.get<RepelResistance>(e2).value;
		float totalResistance = resistance1 + resistance2;

		float moveAmount1 = (resistance2 / totalResistance + 0.01f) * overlap;
		float moveAmount2 = (resistance1 / totalResistance + 0.01f) * overlap;

		if (distance > 0)
		{
			pos.x -= moveAmount1 * (deltaX / distance);
			pos.y -= moveAmount1 * (deltaY / distance);
			otherPos.x += moveAmount2 * (deltaX / distance);
			otherPos.y += moveAmount2 * (deltaY / distance);
		}
		else
		{
			cerr << "Entities are at the same position, cannot resolve." << endl;
		}
	}
	else
	{
		cerr << "No overlap detected, cannot resolve." << endl;
	}
}

const std::vector<CollisionEvent>& CollisionSystem::getCollisionEvents() const
{
	return collisionEvents;
}

CollisionType CollisionSystem::getCollisionType(entt::entity e) const
{
	if (registry.all_of<PlayerTag>(e))
	{
		return CollisionType::Player;
	}

	if (registry.all_of<EnemyTag>(e))
	{
		return CollisionType::Enemy;
	}

	if (registry.all_of<TileTag>(e))
	{
		return CollisionType::Wall;
	}

	if (registry.all_of<SpellTag>(e))
	{
		return CollisionType::Spell;
	}

	return CollisionType::None;
}
