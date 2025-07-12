#pragma once
#include <entt/entity/registry.hpp>
#include <SFML/System/Vector2.hpp>

// PhysicsSystem handles acceleration, force application,
// and physics-based movement

class PhysicsSystem 
{
	entt::registry& registry;
public:
	PhysicsSystem(entt::registry& registry) : registry(registry) {}

	sf::Vector2f calculateAcceleration(entt::entity entity, const sf::Vector2f& force);
	void updateVelocity(float dt);
	void applyRepelForces(float dt);
};