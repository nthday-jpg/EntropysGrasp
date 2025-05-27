#pragma once
#include <entt/entt.hpp>
#include <vector>
#include "../components/collisionComponents.h"
#include "../systems/CollisionSystem.h"

void handleCollision(std::vector<CollisionEvent>& collisionEvents) 
{
	for (auto& event : collisionEvents) 
	{
		auto& A = event.entity1;
		auto& B = event.entity2;
		// call colision manager
		// Here you would typically handle the collision logic, such as applying damage, bouncing entities off each other, etc.
	}
}