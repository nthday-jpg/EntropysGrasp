#include "../systems/CollisionSystem.h"

bool CollisionSystem::isIntersect(Hitbox& hitbox1, Hitbox& hitbox2)
{
	if (hitbox1 == hitbox2)
	{
		return false;
	}

	

}

void CollisionSystem::updateEvents(entt::registry& registry)
{
}

std::vector<CollisionEvent>& CollisionSystem::access()
{
	return collisionEvents;
}
