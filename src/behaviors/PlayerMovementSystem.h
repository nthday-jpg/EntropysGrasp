#pragma once

#include <entt/entt.hpp>

// PlayerMovementSystem.h  
// This header file defines the PlayerMovementSystem structure, which is responsible for handling player movement logic in the game.  
// It includes methods to calculate the player's velocity based on movement direction and speed, as well as a method to compute the player's speed.  
// The system interacts with the ECS registry to access and modify entity components related to movement.  
// The Movement Dirrection and Looking Dirrection be calculated in InputSystem, so this system only calculate the velocity and speed of the player.

// Included headers:  
// - entt: Provides the ECS registry and entity management functionality.  
// - EntityTag.h: Contains tags for identifying entities.  
// - movementComponents.h: Defines components related to movement, such as direction and velocity.  
// - statComponent.h: Includes components for player stats, such as speed.  
// - lookingDirection.h: Contains components for the player's looking direction.

struct PlayerMovementSystem
{

	static void calculateVelo(entt::registry& registry);


	static float calculatedSpeed(entt::registry& registry, entt::entity playerEntity);
};