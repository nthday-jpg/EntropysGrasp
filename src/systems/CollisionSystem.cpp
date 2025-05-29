#include "CollisionSystem.h"
#include "../components/EntityTag.h"
#include <iostream>

using namespace std;


bool CollisionSystem::isIntersect(Hitbox& hitbox1, Hitbox& hitbox2)
{
	// Check if the hitboxes are the same
	if (hitbox1 == hitbox2)
	{
		return false;
	}

	if (hitbox1.type == HitboxType::Rectangle 
		&& hitbox2.type == HitboxType::Rectangle)
	{
		float left1 = hitbox1.offsetX;
		float right1 = left1 + hitbox1.width;
		float top1 = hitbox1.offsetY;
		float bottom1 = top1 + hitbox1.height;
		float left2 = hitbox2.offsetX;
		float right2 = left2 + hitbox2.width;
		float top2 = hitbox2.offsetY;
		float bottom2 = top2 + hitbox2.height;
		return !(left1 > right2 || right1 < left2 
				|| top1 > bottom2 || bottom1 < top2);
	}

	else if (hitbox1.type == HitboxType::Circle 
		&& hitbox2.type == HitboxType::Circle)
	{
		float dx = (hitbox1.offsetX + hitbox1.radius) - (hitbox2.offsetX + hitbox2.radius);
		float dy = (hitbox1.offsetY + hitbox1.radius) - (hitbox2.offsetY + hitbox2.radius);
		float distanceSquared = dx * dx + dy * dy;
		float radiusSum = hitbox1.radius + hitbox2.radius;
		return distanceSquared <= (radiusSum * radiusSum);
	}

	else
	{
		if (hitbox1.type == HitboxType::Rectangle)
		{
			float circleX = hitbox2.offsetX + hitbox2.radius;
			float circleY = hitbox2.offsetY + hitbox2.radius;
			float rectLeft = hitbox1.offsetX;
			float rectRight = rectLeft + hitbox1.width;
			float rectTop = hitbox1.offsetY;
			float rectBottom = rectTop + hitbox1.height;
			if (circleX <= rectLeft || circleX >= rectRight ||
				circleY <= rectTop || circleY >= rectBottom)
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
			return isIntersect(hitbox2, hitbox1); // Swap the order
		}
	}
}

void CollisionSystem::collectCollision()
{
	collisionEvents.clear();
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

			auto& otherHitbox = registry.get<Hitbox>(otherEntity);
			if (isIntersect(hitbox, otherHitbox))
			{
				CollisionType type1 = getCollisionType(entity);
				CollisionType type2 = getCollisionType(otherEntity);

				collisionEvents.emplace_back(entity, otherEntity, type1, type2);
			}
		}
	}
}

void CollisionSystem::resolveCollsions()
{
	for (auto& event : collisionEvents)
	{
		using CT = CollisionType;

		/*
		* Player = 0
		* Enemy = 1
		* Spell = 2
		* Wall = 3
		* None = 4
		* Ensure type1 is always less than type2 for consistent handling
		*/
		CT type1 = event.type1;
		CT type2 = event.type2;
		if (type1 > type2)
		{
			swap(type1, type2);
			swap(event.entity1, event.entity2);
		}
		

		switch (type1) {
			case CT::Player:
				switch (type2) {
					case CT::Enemy:
						cout << "Player collided with Enemy" << endl;
						break;

					case CT::Spell:
						cout << "Player collided with Spell" << endl;
						break;

					case CT::Wall:
						cout << "Player collided with Wall" << endl;
						break;
				}
				break;
			
			case CT::Enemy:
				switch (type2) {
					case CT::Enemy:
						cout << "Enemy collided with Enemy" << endl;
						break;

					case CT::Spell:
						cout << "Enemy collided with Spell" << endl;
						break;

					case CT::Wall:
						cout << "Enemy collided with Wall" << endl;
						break;

				}
				break;

			case CT::Spell:
				switch (type2) {
					case CT::Wall:
						cout << "Spell collided with Wall" << endl;
						break;
				}
				break;

			case CT::None:
				//throw an error or log a warning
				break;
		}

	}
}

CollisionType CollisionSystem::getCollisionType(entt::entity e) {
	if (registry.all_of<PlayerTag>(e))
	{
		return CollisionType::Player;
	}

	if (registry.all_of<EnemyTag>(e))  
	{
		return CollisionType::Enemy;
	}

	if (registry.all_of<WallTag>(e))  
	{
		return CollisionType::Wall;
	}

	if (registry.all_of<SpellTag>(e)) 
	{
		return CollisionType::Spell;
	}

	return CollisionType::None;
}
