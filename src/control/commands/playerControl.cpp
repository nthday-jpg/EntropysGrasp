#include "PlayerControl.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>

#include "../../manager/WindowManager.h"
#include "../../gameplay/components/MovementComponents.h"
#include "../../gameplay/components/LookingDirection.h"
#include "../../gameplay/components/EntityTags.h"
#include "../../gameplay/components/Spell.h"
#include "../../gameplay/systems/SpellManager.h"
#include "../../manager/SpellLibrary.h"
#include "../../gameplay/systems/AnimationSystem.h"

#include "../../utils/VectorMath.h"
#include "../../utils/Random.h"


const MovementDirection down = MovementDirection(0, 1);
const MovementDirection up = MovementDirection(0, -1);
const MovementDirection left = MovementDirection(-1, 0);
const MovementDirection right = MovementDirection(1, 0);

// Helper function to convert LookingDirection to Direction enum for animation
Direction getDirectionFromLooking(const LookingDirection& lookingDir) {
	float x = lookingDir.x;
	float y = lookingDir.y;
	// x axis is horizontal and 
	return Direction::Down;
}

// Helper function to check if player is currently moving
bool isPlayerMoving(entt::registry& registry, entt::entity playerEntity) {
	if (registry.all_of<MovementDirection>(playerEntity)) {
		const MovementDirection& moveDir = registry.get<MovementDirection>(playerEntity);
		return (std::abs(moveDir.x) > 0.01f || std::abs(moveDir.y) > 0.01f);
	}
	return false;
}

void MoveDown::execute(entt::registry& registry)
{
	if (!registry.all_of<MovementDirection, PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not have MovementDirection or PlayerTag component." << std::endl;
		return;
	}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += down;
	// Animation is handled by LookAtMouse command based on where player is looking
}

void MoveUp::execute(entt::registry& registry)
{
	if (!registry.all_of<MovementDirection, PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not have MovementDirection or PlayerTag component." << std::endl;
		return;
	}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += up;
	// Animation is handled by LookAtMouse command based on where player is looking
}

void MoveLeft::execute(entt::registry& registry)
{
	if (!registry.all_of<MovementDirection, PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not have MovementDirection or PlayerTag component." << std::endl;
		return;
	}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += left;
	// Animation is handled by LookAtMouse command based on where player is looking
}

void MoveRight::execute(entt::registry& registry)
{
	if (!registry.all_of<MovementDirection, PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not have MovementDirection or PlayerTag component." << std::endl;
		return;
	}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += right;
	// Animation is handled by LookAtMouse command based on where player is looking
}

void CastSpell::execute(entt::registry& registry)
{
	spellManager->castSpell(spellManager->currentSpell());
	
	// Trigger casting animation based on looking direction
	if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
		Direction castDirection = Direction::Down; // Default
		if (registry.all_of<LookingDirection>(playerEntity)) {
			LookingDirection& lookDir = registry.get<LookingDirection>(playerEntity);
			castDirection = getDirectionFromLooking(lookDir);
		}

		(*dispatcher)->enqueue<AnimationChangeEvent>({ playerEntity, AnimationState::Attacking, castDirection });
	}
}

void ChangeSpell::execute(entt::registry& registry)
{
	spellManager->setCurrentSpell(spellID);
}

void Dash::execute(entt::registry& registry)
{
	// Trigger dash animation based on looking direction
	if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
		Direction dashDirection = Direction::Down; // Default
		if (registry.all_of<LookingDirection>(playerEntity)) {
			LookingDirection& lookDir = registry.get<LookingDirection>(playerEntity);
			dashDirection = getDirectionFromLooking(lookDir);
		}

		(*dispatcher)->enqueue<AnimationChangeEvent>({ playerEntity, AnimationState::Dashing, dashDirection });
	}
}

void ResetTempComponents::execute(entt::registry& registry)
{
	if (!registry.all_of<PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not PlayerTag component." << std::endl;
		return;
	}

	// Store current looking direction before reset
	LookingDirection currentLookingDir(0.0f, 0.0f);
	if (registry.all_of<LookingDirection>(playerEntity)) {
		currentLookingDir = registry.get<LookingDirection>(playerEntity);
	}

	// Reset movement direction (this stops movement)
	if (!registry.all_of<MovementDirection>(playerEntity))
	{
		registry.emplace<MovementDirection>(playerEntity, MovementDirection(0, 0));
	}
	else
	{
		registry.get<MovementDirection>(playerEntity) = MovementDirection(0, 0);
	}

	if (!registry.all_of<LookingDirection>(playerEntity))
	{
		registry.emplace<LookingDirection>(playerEntity, LookingDirection{ 0.0f,0.0f });
	}

	if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
		Direction idleDirection = getDirectionFromLooking(currentLookingDir);
		(*dispatcher)->enqueue<AnimationChangeEvent>({ playerEntity, AnimationState::Idle, idleDirection });
	}
}

void LookAtMouse::execute(entt::registry& registry)
{
	if (!registry.all_of<LookingDirection, PlayerTag, Position>(playerEntity))
	{
		std::cerr << "Player entity does not have LookingDirection or PlayerTag or Position component." << std::endl;
		return;
	}

	WindowManager& windowManager = WindowManager::getInstance();
	if (!windowManager.hasWindow() || !windowManager.isOpen())
	{
		std::cerr << "Window is not open or does not exist." << std::endl;
		return;
	}
	Direction oldDirection = Direction::Down;
	if (registry.all_of<Direction>(playerEntity)) {
		oldDirection = registry.get<Direction>(playerEntity);
	}

	Position& playerPosition = registry.get<Position>(playerEntity);
	sf::Vector2i mousePosWindow = sf::Mouse::getPosition(windowManager.getWindow());
	sf::Vector2f mousePos = windowManager.getWindow().mapPixelToCoords(mousePosWindow);

	LookingDirection& lookingDirection = registry.get<LookingDirection>(playerEntity);
	lookingDirection = LookingDirection{
		static_cast<float>(mousePos.x - playerPosition.x),
		static_cast<float>(mousePos.y - playerPosition.y) };

	normalize(lookingDirection);

	// Calculate the angle difference to avoid unnecessary animation updates

	Direction newDirection = getDirectionFromLooking(lookingDirection);
	if (newDirection != oldDirection) {
		if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>())
		{
			(*dispatcher)->enqueue<AnimationChangeEvent>({ playerEntity, AnimationState::Walking, newDirection });

		}
	}
	registry.emplace_or_replace<LookingDirection>(playerEntity, lookingDirection);
	registry.emplace_or_replace<Direction>(playerEntity, newDirection);
}