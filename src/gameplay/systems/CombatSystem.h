#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <iostream>
#include "../systems/CollisionSystem.h"
#include "../../manager/SpellLibrary.h"

class CombatSystem 
{
	entt::registry& registry;
	entt::dispatcher& dispatcher;
public:
	void applyDamage(float dmg, entt::entity);
	void handlePlayerEnemyCollision(entt::entity player, entt::entity enemy);
	void handleEnemySpellCollision(entt::entity enemy, entt::entity spell, SpellLibrary spellLibrary);
};