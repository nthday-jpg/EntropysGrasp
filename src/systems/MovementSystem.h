#pragma once  
#include <entt/entt.hpp>  

// The MovementSystem struct is responsible for updating the movement-related components  
// of entities within the registry. It operates on entities that have specific components  
// related to movement and performs updates based on the provided deltaTime.  

// Included headers:
// -entt : Provides the ECS registry and entity management functionality.
// - movementComponents.h : Defines components related to movement, such as Position and Velocity.

struct MovementSystem  
{  
	// Updates the positions of entities based on their velocities and the elapsed time.
   static void update(entt::registry& registry, float deltaTime);  
};
