#include "PlayerMovementSystem.h"

#include "../components/EntityTags.h"
#include "../components/movementComponents.h"
#include "../components/statComponent.h" 
#include "../components/lookingDirection.h"

void PlayerMovementSystem::calculateVelo(entt::registry& registry)
{
	auto view = registry.view<PlayerTag>();
	for (auto playerEntity : view)
	{
		auto speed = calculatedSpeed(registry, playerEntity);
		auto movementDirection = registry.get<MovementDirection>(playerEntity);
		Velocity velocity = { movementDirection.x * speed, movementDirection.y * speed };
		registry.emplace_or_replace<Velocity>(playerEntity, velocity);
	}
}

float PlayerMovementSystem::calculatedSpeed(entt::registry& registry, entt::entity playerEntity)
{
	//movement and looking is normalized vectors length 1
	LookingDirection looking = registry.get<LookingDirection>(playerEntity);
	MovementDirection movement = registry.get<MovementDirection>(playerEntity);
	float speed = registry.get<Speed>(playerEntity).value;

	// Using dot product to scale speed 
	// based on how much the movement direction and looking direction align
	speed = speed * (0.8 + 0.2 * (looking.x * movement.x + looking.y * movement.y));

	return speed;
}