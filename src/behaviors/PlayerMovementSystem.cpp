#include "PlayerVelocityController.h"

#include "../components/EntityTags.h"
#include "../components/movementComponents.h"
#include "../components/statComponent.h" 
#include "../components/lookingDirection.h"
#include "../Utils/VectorMath.h"

void PlayerVelocityController::calculateVelo(entt::registry& registry)
{
	auto view = registry.view<PlayerTag>();
	for (auto playerEntity : view)
	{
		auto speed = calculatedSpeed(registry, playerEntity);
		auto movementDirection = registry.get<MovementDirection>(playerEntity);

		normalize(movementDirection);
		Velocity velocity = { movementDirection.x * speed, movementDirection.y * speed };
		registry.emplace_or_replace<Velocity>(playerEntity, velocity);
	}
}

float PlayerVelocityController::calculatedSpeed(entt::registry& registry, entt::entity playerEntity)
{
	if (!registry.all_of<MovementDirection, LookingDirection, Speed>(playerEntity))
	{
		throw std::runtime_error("Player entity does not have all required components for speed calculation.");
	}

	//movement and looking is normalized vectors length 1
	LookingDirection looking = registry.get<LookingDirection>(playerEntity);
	MovementDirection movement = registry.get<MovementDirection>(playerEntity);
	float speed = registry.get<Speed>(playerEntity).value;

	
	// Using dot product to scale speed 
	// based on how much the movement direction and looking direction align
	speed = speed * (0.6 + 0.4 * (looking.x * movement.x + looking.y * movement.y));

	return speed;
}