#pragma once

#include <entt/entt.hpp>

// PlayerMovementSystem.h  
// This header file defines the PlayerMovementSystem structure,
// which is responsible for handling player movement logic in the game.  
// It includes methods to calculate the player's velocity based on movement direction and speed,
// as well as a method to compute the player's speed.  
// The system interacts with the ECS registry to access and modify entity components related to movement.  
// The Movement Dirrection and Looking Dirrection be calculated in InputSystem,
// so this system only calculate the velocity and speed of the player.


struct PlayerMovementSystem
{

	// Calculate the velocity of the player based on movement direction and speed.
	// The required components are:
	// - PlayerTag
	// - MovementDirection
	// - Speed
	// The exception is thrown by `calculatedSpeed` 
	// if the player entity does not have all required components.
	static void calculateVelo(entt::registry& registry);

	// Calculate the speed base on the player's movement direction and looking direction.
	// The required components are:
	// - MovementDirection
	// - LookingDirection
	// - Speed
	// If the player entity does not have all required components, an exception is thrown.
	static float calculatedSpeed(entt::registry& registry, entt::entity playerEntity);
};