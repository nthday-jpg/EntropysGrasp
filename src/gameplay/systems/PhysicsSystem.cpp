#include "PhysicsSystem.h"
#include "../components/Hitbox.h"
#include "../components/MovementComponents.h"
#include "../components/StatComponent.h"
#include "../components/EffectTags.h"
#include <iostream>

sf::Vector2f PhysicsSystem::calculateAcceleration(entt::entity entity, const sf::Vector2f& force)
{
	if (!registry.valid(entity))
	{
		return sf::Vector2f(0.0f, 0.0f); // Return zero vector instead of void
	}
	if (!registry.any_of<Velocity, RepelResistance>(entity))
	{
		std::cerr << "Entity does not have a Velocity or RepelResistance component." << std::endl;
		return sf::Vector2f(0.0f, 0.0f);
	}

	RepelResistance& resistance = registry.get<RepelResistance>(entity);
	
	sf::Vector2f accel = {force.x / (1 + resistance.value), force.y / (1 + resistance.value)};
	return accel;
}

void PhysicsSystem::updateVelocity(float dt)
{
	// Apply repel forces to velocity
	applyRepelForces(dt);
	
	// Add other physics-based velocity modifications here
	// (gravity, friction, other forces, etc.)
}

void PhysicsSystem::applyRepelForces(float dt)
{
	auto view = registry.view<RepelTag, Velocity, RepelResistance>();
	for (auto [entity, repelTag, velocity, resistance] : view.each())
	{
		// Calculate force with time decay
		sf::Vector2f currentForce = repelTag.force * (1.0f + repelTag.remainingTime);
		
		// Apply force through physics system for consistency
		sf::Vector2f acceleration = calculateAcceleration(entity, currentForce);
		
		// Apply acceleration to velocity
		velocity.x += acceleration.x * dt;
		velocity.y += acceleration.y * dt;
	}
}