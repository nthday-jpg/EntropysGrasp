#include "PositionIntegrator.h"
#include "../components/MovementComponents.h"  


void PositionIntegrator::update(entt::registry& registry, float dt)
{
	auto view = registry.view<Position, Velocity>();
	for (auto [entity, position, velocity] : view.each())
	{
		position += velocity * dt;
	}
}