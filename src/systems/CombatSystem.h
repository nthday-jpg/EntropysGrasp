#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <iostream>
#include "../systems/CollisionSystem.h"
#include "../resources/SpellLibrary.h"

class CombatSystem {
	entt::registry& registry;
	entt::dispatcher& dispatcher;
public:
	void handleCollision(std::vector<CollisionEvent>& collisionEvents);
	void resolveEnemySpellCollision();
	void applyDamage(float dmg, entt::entity);
	void handlePlayerEnemyCollision(entt::entity player, entt::entity enemy);
	void handleEnemySpellCollision(entt::entity enemy, entt::entity spell, SpellLibrary spellLibrary);
};