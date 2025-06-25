#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include "../behaviors/EntityBehavior.h"
#include "../resources/SpellLibrary.h"
#include "../resources/EnemyLibrary.h"

using BehaviorFunction = std::function<void(entt::entity, entt::entity, entt::registry&, float, const SpellLibrary&, const EnemyLibrary&)>;

class BehaviorSystem {
	std::unordered_map<BehaviorType, BehaviorFunction> behaviorMap;
public:
	void initializeBehaviorMap();
	void updateBehavior(entt::registry& registry, float dt, const SpellLibrary& spellLibrary, const EnemyLibrary& enemyLibrary);
};

