#pragma once  
#include <entt/entt.hpp>  


// The MovementSystem struct is responsible for updating the movement-related components  
// of entities within the registry. It operates on entities that have specific components  
// related to movement and performs updates based on the provided deltaTime.  
struct MovementSystem  
{  
   // Updates the movement components of entities in the registry.  
   // Parameters:  
   // - registry: The entity registry containing all entities and their components.  
   // - deltaTime: The time elapsed since the last update, used for calculating movement.  
   void update(entt::registry& registry, float deltaTime);  
};
