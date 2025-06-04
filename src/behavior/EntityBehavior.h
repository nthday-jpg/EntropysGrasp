#pragma once
#include <entt/entt.hpp>
#include <cmath>
#include "../components/movementComponents.h"
#include "../components/Spell.h"

struct StraightMovement {};
struct HomingMovement { entt::entity target; };
struct OrbitMovement { entt::entity center; float radius; float speed; };

void straightMovementBehavior(entt::entity spell, entt::registry& registry, float dt) {
	auto& position = registry.get<Position>(spell);
	auto& velo = registry.get<Velocity>(spell);
	auto& spellData = registry.get<SpellData>(spell);
	auto& direction = registry.get<MovementDirection>(spell);

	velo.x = direction.x * spellData.speed;
	velo.y = direction.y * spellData.speed;
	position += velo * dt;
}

void homingMovementBehavior(entt::entity spell, entt::entity enemy, entt::registry& registry, float dt) {
	auto& position = registry.get<Position>(spell);
	auto& velo = registry.get<Velocity>(spell);
	auto& spellData = registry.get<SpellData>(spell);
	auto& enemyPosition = registry.get<Position>(enemy);

	// Calculate direction towards the enemy
	auto direction = enemyPosition - position;
	float distance = sqrt(pow(direction.x, 2) + pow(direction.y, 2));

	if (distance > 0) {
		direction /= distance; // Normalize the direction vector
		velo.x = direction.x * spellData.speed;
		velo.y = direction.y * spellData.speed;
	}
	position += velo * dt; // Move the spell towards the enemy
}

void orbitMovementBehavior(entt::entity entity, entt::registry& registry, float dt) {
	auto& position = registry.get<Position>(entity);
	auto& spellData = registry.get<SpellData>(entity);
	auto& orbit = registry.get<OrbitMovement>(entity);
	auto& centerPosition = registry.get<Position>(orbit.center);
	// Calculate the angle of the orbit
	float angle = dt * orbit.speed;

	// Update position based on orbit radius and angle
	position.x = centerPosition.x + orbit.radius * cos(angle);
	position.y = centerPosition.y + orbit.radius * sin(angle);
}