#include "ParticleSystem.h"
#include "../../utils/Random.h"
#include "../components/MovementComponents.h" // For Position component

ParticleSystem::ParticleSystem(entt::registry& registry)
    : registry(registry)
{
	initializeBehaviorMap();
}

void ParticleSystem::initializeBehaviorMap()
{
	behaviorMap[ParticleBehaviorType::Linear] = Linear::create();
	behaviorMap[ParticleBehaviorType::Floating] = Floating::create();
	behaviorMap[ParticleBehaviorType::Orbit] = Orbit::create();
}

void ParticleSystem::emit(const ParticleProperties& properties)
{
	auto entity = registry.create();
	
	// Add particle-specific component
	Velocity finalVelocity =  Velocity(
		properties.velocity.x + Random::getFloat(-properties.velocityVariation.x, properties.velocityVariation.x),
		properties.velocity.y + Random::getFloat(-properties.velocityVariation.y, properties.velocityVariation.y)
	);

	registry.emplace<Position>(entity, properties.position);
	registry.emplace<Velocity>(entity, finalVelocity);
	
	registry.emplace<ParticleComponent>
	(	
		entity, 
		properties.startColor,
		properties.endColor,
		properties.sizeStart,
		properties.sizeEnd,
		properties.lifetime
	);

}

void ParticleSystem::update(float dt)
{
	auto view = registry.view<ParticleComponent>();
	for (auto entity : view) {
		auto& particle = view.get<ParticleComponent>(entity);
		auto& position = registry.get<Position>(entity);
		auto& velocity = registry.get<Velocity>(entity);
		if (registry.any_of<inativeParticle>(entity)) {
			continue;
		}

		// Update age and check lifetime
		particle.age += dt;
		if (particle.age >= particle.lifetime) {
			registry.emplace<inativeParticle>(entity);
			continue;
		}

		// Call the behavior function based on the type
		auto behaviorFunc = behaviorMap[particle.behaviorType];
		if (behaviorFunc) {
			behaviorFunc(entity, particle, position, velocity, dt);
		}
	}
}