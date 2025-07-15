#include "ParticleBehaviors.h"
#include "../../utils/Random.h"


ParticleBehaviorFunction Linear::create()
{
	return [](entt::entity entity, ParticleComponent& particle, Position& position, Velocity& velocity, float dt) 
		{
		
		};
}

ParticleBehaviorFunction Floating::create()
{
	return [](entt::entity entity, ParticleComponent& particle, Position& position, Velocity& velocity, float dt)
		{
			// Random drift
			velocity.x += Random::getFloat(-10.0f, 10.0f);
			velocity.y += Random::getFloat(-10.0f, 10.0f);
		};
}

ParticleBehaviorFunction Orbit::create()
{
	return [](entt::entity entity, ParticleComponent& particle, Position& position, Velocity& velocity, float dt) 
		{
			float angle = dt * 2.0f * 3.14159f / particle.lifetime; // Full circle in lifetime seconds
			float radius = 50.0f; // Fixed radius for simplicity
			//position.x += radius * cos(angle);
			//position.y += radius * sin(angle);
		};
}