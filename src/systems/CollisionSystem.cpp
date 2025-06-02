#include "CollisionSystem.h"
#include "../components/EntityTag.h"
#include "../components/statComponent.h"
#include <iostream>

using namespace std;


#include "CollisionSystem.h"  
#include "../components/EntityTag.h"  
#include <iostream>  

using namespace std;  

bool CollisionSystem::isIntersect(entt::entity e1, entt::entity e2)  
{  
	auto& hitbox1 = registry.get<Hitbox>(e1);  
	auto& hitbox2 = registry.get<Hitbox>(e2);  

	auto& pos1 = registry.get<Position>(e1);  
	auto& pos2 = registry.get<Position>(e2);  

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

	if (hitbox1.type == HitboxType::Rectangle   
		&& hitbox2.type == HitboxType::Rectangle)  
	{  
		float left1 = hitbox1PosX;  
		float right1 = left1 + hitbox1.width;  
		float top1 = hitbox1PosY;  
		float bottom1 = top1 + hitbox1.height;  
		float left2 = hitbox2PosX;  
		float right2 = left2 + hitbox2.width;  
		float top2 = hitbox2PosY;  
		float bottom2 = top2 + hitbox2.height;  
		return !(left1 > right2 || right1 < left2   
				|| top1 > bottom2 || bottom1 < top2);  
	}  

	else if (hitbox1.type == HitboxType::Circle   
			&& hitbox2.type == HitboxType::Circle)  
	{  
		float dx = hitbox1PosX - hitbox2PosX;  
		float dy = hitbox1PosY - hitbox2PosY;  
		float distanceSquared = dx * dx + dy * dy;  
		float radiusSum = hitbox1.radius + hitbox2.radius;  
		return distanceSquared <= (radiusSum * radiusSum);  
	}  

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
			return isIntersect(e2, e1); // Swap the order  
		}  
	}  
}

void CollisionSystem::detectCollisions()
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
			if (isIntersect(entity, otherEntity))
			{
				CollisionType type1 = getCollisionType(entity);
				CollisionType type2 = getCollisionType(otherEntity);

				collisionEvents.emplace_back(entity, otherEntity, type1, type2);
			}
		}
	}
}

void CollisionSystem::resolvePhysicalOverlap(entt::entity e1, entt::entity e2)
{
	auto type1 = getCollisionType(e1);
	auto type2 = getCollisionType(e2);


	Hitbox& hitbox = registry.get<Hitbox>(e1);
	Hitbox& otherHitbox = registry.get<Hitbox>(e2);

	if (hitbox.type != HitboxType::Rectangle || otherHitbox.type != HitboxType::Rectangle)
	{
		cerr << "Currently only rectangle hitboxes are supported for resolving overlap." << endl;
		return; 
	};

	float resistance1 = registry.get<ResistanceComponent>(e1).value; 
	float resistance2 = registry.get<ResistanceComponent>(e2).value;
	float totalResistance = resistance1 + resistance2;


	//0.01f is added to ensure two entities will not be completely stuck together
	float recoil1 = (resistance2 / totalResistance + 0.01f);
	float recoil2 = (resistance1 / totalResistance + 0.01f);

	float centerX1 = hitbox.offsetX + hitbox.width / 2.0f;
	float centerY1 = hitbox.offsetY + hitbox.height / 2.0f;

	float centerX2 = otherHitbox.offsetX + otherHitbox.width / 2.0f;
	float centerY2 = otherHitbox.offsetY + otherHitbox.height / 2.0f;

	float deltaX = centerX2 - centerX1;
	float deltaY = centerY2 - centerY1;

	float overlapX = (hitbox.width + otherHitbox.width) / 2.0f - std::abs(deltaX);
	float overlapY = (hitbox.height + otherHitbox.height) / 2.0f - std::abs(deltaY);

	if (overlapX < 0 || overlapY < 0)
	{
		cerr << "No overlap detected, cannot resolve." << endl;
		return; // No overlap to resolve
	}


	else if (overlapX < overlapY)
	{
		// Resolve along X axis
		float moveAmount1 = recoil1 * overlapX;
		float moveAmount2 = recoil2 * overlapX;

		if (deltaX > 0)
		{
			hitbox.offsetX -= moveAmount1; // Move e1 left
			otherHitbox.offsetX += moveAmount2; // Move e2 right
		}
		else
		{
			hitbox.offsetX += moveAmount1; // Move e1 right
			otherHitbox.offsetX -= moveAmount2; // Move e2 left
		}
	}
	else
	{
		// Resolve along Y axis
		float moveAmount1 = recoil1 * overlapY;
		float moveAmount2 = recoil2 * overlapY;

		if (deltaY > 0)
		{
			hitbox.offsetY -= moveAmount1; // Move e1 up
			otherHitbox.offsetY += moveAmount2; // Move e2 down
		}
		else
		{
			hitbox.offsetY += moveAmount1; // Move e1 down
			otherHitbox.offsetY -= moveAmount2; // Move e2 up
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

