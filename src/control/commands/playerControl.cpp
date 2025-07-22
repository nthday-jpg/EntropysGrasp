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


const MovementDirection down = MovementDirection(0, 1);
const MovementDirection up = MovementDirection(0, -1);
const MovementDirection left = MovementDirection(-1, 0);
const MovementDirection right = MovementDirection(1, 0);

// Helper function to convert LookingDirection to Direction enum for animation
Direction getDirectionFromLooking(const LookingDirection& lookingDir) {
    float x = lookingDir.x;
    float y = lookingDir.y;
    
    // If looking direction is too small, return a default direction
    if (std::abs(x) < 0.1f && std::abs(y) < 0.1f) {
        return Direction::Down; // Default when not looking anywhere specific
    }
    
    // Calculate angle to determine exact direction
    float angle = std::atan2(y, x) * 180.0f / 3.14159265359f; // Convert to degrees
    
    // Normalize angle to 0-360 range
    if (angle < 0) angle += 360.0f;
    
    // Map angle to 8 directions (each direction covers 45 degrees)
    // Right = 0°, DownRight = 45°, Down = 90°, DownLeft = 135°, 
    // Left = 180°, UpLeft = 225°, Up = 270°, UpRight = 315°
    
    if (angle >= 337.5f || angle < 22.5f) return Direction::Right;        // 0° ± 22.5°
    else if (angle >= 22.5f && angle < 67.5f) return Direction::DownRight; // 45° ± 22.5°
    else if (angle >= 67.5f && angle < 112.5f) return Direction::Down;     // 90° ± 22.5°
    else if (angle >= 112.5f && angle < 157.5f) return Direction::DownLeft; // 135° ± 22.5°
    else if (angle >= 157.5f && angle < 202.5f) return Direction::Left;     // 180° ± 22.5°
    else if (angle >= 202.5f && angle < 247.5f) return Direction::UpLeft;   // 225° ± 22.5°
    else if (angle >= 247.5f && angle < 292.5f) return Direction::Up;       // 270° ± 22.5°
    else if (angle >= 292.5f && angle < 337.5f) return Direction::UpRight;  // 315° ± 22.5°
    
    return Direction::Down; // Default fallback
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
	ParticleProperties particleProperties;
	particleProperties.startColor = sf::Color::Red;
	particleProperties.endColor = sf::Color::Yellow;
	particleProperties.sizeEnd = 0.0f;
	particleProperties.sizeStart = 5.0f;
	particleProperties.lifetime = 5.0f;
	particleProperties.velocity = { 0.0f, 0.f };
	particleProperties.velocityVariation = { -2.0f, 2.0f };
	particleProperties.behaviorType = ParticleBehaviorType::Floating;

	for (int i = 0; i < 10; i++)
	{
		particleSystem->emit(particleProperties);
	}
	
	// Trigger casting animation based on looking direction
	if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
		Direction castDirection = Direction::Down; // Default
		if (registry.all_of<LookingDirection>(playerEntity)) {
			LookingDirection& lookDir = registry.get<LookingDirection>(playerEntity);
			castDirection = getDirectionFromLooking(lookDir);
		}
		
		(*dispatcher)->enqueue<AnimationChangeEvent>(playerEntity, AnimationState::Attacking, castDirection);
	}
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
		
		(*dispatcher)->enqueue<AnimationChangeEvent>(playerEntity, AnimationState::Dashing, dashDirection);
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
		(*dispatcher)->enqueue<AnimationChangeEvent>(playerEntity, AnimationState::Idle, idleDirection);
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

	Position& playerPosition = registry.get<Position>(playerEntity);
	sf::Vector2i mousePosWindow = sf::Mouse::getPosition(windowManager.getWindow());
	sf::Vector2f mousePos = windowManager.getWindow().mapPixelToCoords(mousePosWindow);

	// Store previous looking direction to check if it changed significantly
	LookingDirection previousLookingDir = registry.get<LookingDirection>(playerEntity);

	LookingDirection& lookingDirection = registry.get<LookingDirection>(playerEntity);
	lookingDirection = LookingDirection{
		static_cast<float>(mousePos.x - playerPosition.x),
		static_cast<float>(mousePos.y - playerPosition.y) };
	
	normalize(lookingDirection);

	// Calculate the angle difference to avoid unnecessary animation updates
	float angleDiff = std::atan2(lookingDirection.y, lookingDirection.x) - std::atan2(previousLookingDir.y, previousLookingDir.x);
	if (angleDiff > 3.14159265359f) angleDiff -= 2 * 3.14159265359f;
	if (angleDiff < -3.14159265359f) angleDiff += 2 * 3.14159265359f;
	
	// Only update animation if the direction changed significantly (more than ~11.25 degrees)
	if (std::abs(angleDiff) > 0.196f) { // 0.196 radians ≈ 11.25 degrees
		if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
			Direction newDirection = getDirectionFromLooking(lookingDirection);
			
			// Choose animation state based on whether player is moving
			AnimationState animState = isPlayerMoving(registry, playerEntity) ? 
				AnimationState::Walking : AnimationState::Idle;
			
			(*dispatcher)->enqueue<AnimationChangeEvent>(playerEntity, animState, newDirection);
		}
	}
}
