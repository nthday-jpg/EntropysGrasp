#pragma once  
#include <entt/entt.hpp>  

// The PositionIntegrator system is responsible for updating 
// the positions of entities based on their velocities and the elapsed time.  

struct PositionIntegrator
{
	// Updates the positions of entities based on their velocities and the elapsed time.
	static void update(entt::registry& registry, float deltaTime);
};