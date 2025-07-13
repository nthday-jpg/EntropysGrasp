#pragma once

#include <entt/entt.hpp>

// This header file defines the PlayerVelocityControllerstructure,
// which is responsible for handling player movement logic in the game.  
// It includes methods to calculate the player's velocity based on movement direction and speed,
// as well as a method to compute the player's speed.  
// The system interacts with the ECS registry to access and modify entity components related to movement.  
// The Movement Dirrection and Looking Dirrection be calculated in InputSystem,
// so this system only calculate the velocity and speed of the player.


class PlayerVelocityController
{
public:

	// Calculate the velocity of the player based on movement direction and speed.
	// The required components are:
	// - PlayerTag
	// - MovementDirection
	// - Speed
	// The exception is thrown by `calculatedSpeed` 
	// if the player entity does not have all required components.
	static void calculateVelocity(entt::registry& registry);

	// Calculate the speed base on the player's movement direction and looking direction.
	// The required components are:
	// - MovementDirection
	// - LookingDirection
	// - Speed
	// If the player entity does not have all required components, an exception is thrown.
	static float calculateSpeed(entt::registry& registry,entt::entity playerEntity);
};