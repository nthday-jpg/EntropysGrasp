#include <entt/entt.hpp>
#include "MovementSystem.h"
#include "../components/EntityTag.h"
#include "../components/lookingDirection.h"
#include "../components/movementComponents.h"

//cho speed và có thể phát triển để có tốc độ và di chuyển theo máu
#include "../components/statComponent.h" 

//Xử lý chuyển động ở đây
void PlayerMovementSystem::update(entt::registry& registry, float dt)
{
	auto view = registry.view<PlayerTag>();
	for (auto entity : view) {
		auto& position = registry.get<Position>(entity);

		float calculatedSpeed = PlayerMovementSystem::calculatedSpeed(registry, entity);
		position += registry.get<MovementDirection>(entity) * calculatedSpeed * dt;
	}
}

float PlayerMovementSystem::calculatedSpeed(entt::registry& registry, entt::entity playerEntity)
{
	//return calculated speed based on health, mana, terrain, etc.
	return 0;
}


void EnemyMovementSystem::update(entt::registry& registry, float dt)
{
	
}