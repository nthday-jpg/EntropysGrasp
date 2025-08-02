#include "PositionIntegrator.h"
#include "../components/MovementComponents.h" 
#include "../components/EntityTags.h"

void PositionIntegrator::update(entt::registry& registry, float dt)
{
	auto view = registry.view<Position, Velocity>(entt::exclude<Inactive>);
	for (auto [entity, position, velocity] : view.each())
	{
		position += velocity * dt;
	}
}