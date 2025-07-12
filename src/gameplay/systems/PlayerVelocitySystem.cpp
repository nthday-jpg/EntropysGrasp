#include "PlayerVelocitySystem.h"

#include "../components/EntityTags.h"
#include "../components/movementComponents.h"
#include "../components/statComponent.h" 
#include "../components/lookingDirection.h"
#include "../../utils/VectorMath.h"
#include <stdexcept>

void PlayerVelocityController::calculateVelocity(entt::registry& registry)
{
	auto view = registry.view<PlayerTag>();
	for (entt::entity playerEntity : view)
	{
		float speed = calculateSpeed(registry, playerEntity);
		MovementDirection movementDirection = registry.get<MovementDirection>(playerEntity);

		normalize(static_cast<sf::Vector2f&>(movementDirection));
		Velocity velocity = { movementDirection.x * speed, movementDirection.y * speed };
		registry.emplace_or_replace<Velocity>(playerEntity, velocity);
	}
}

float PlayerVelocityController::calculateSpeed(entt::registry& registry, entt::entity playerEntity)
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
	speed = speed * (0.8 + 0.2 * (looking.x * movement.x + looking.y * movement.y));

	return speed;
}