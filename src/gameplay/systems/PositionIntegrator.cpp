#include "PositionIntegrator.h"
#include "../components/MovementComponents.h"  
#include "../components/EntityTags.h"
#include "../components/Particle.h"

void PositionIntegrator::update(entt::registry& registry, float dt)
{
	auto view = registry.view<Position, Velocity>();
	for (auto [entity, position, velocity] : view.each())
	{
		if (registry.any_of<InactiveEnemyTag, InactiveParticle>(entity))
		{
			continue;
		}
		position += velocity * dt;
	}
}