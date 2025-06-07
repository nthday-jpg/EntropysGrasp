#pragma once
#include <entt/entt.hpp>
#include <vector>
#include "../systems/CollisionSystem.h"
#include <iostream>

// This file defines the CombatSystem class which handles combat-related logic
// including collision handling, damage application, and spell effects.


class CombatSystem {
	entt::registry& registry;
	entt::dispatcher& dispatcher;
	 
public:
	void handleCollision(std::vector<CollisionEvent>& collisionEvents);
	void resolveEnemySpellCollision();
	void applyDamage(float dmg, entt::entity);

	CombatSystem(entt::registry& reg, entt::dispatcher& disp)
		: registry(reg), dispatcher(disp) {
	}
};

