#include "MovementSystem.h"
#include "../components/movementComponents.h"  


void MovementSystem::update(entt::registry& registry, float dt)
{
	auto view = registry.view<Position, Velocity>();
	for (auto [entity, position, velocity] : view.each())
	{
		position += velocity * dt;
	}
}