#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include "../behavior/EntityBehavior.h"

using BehaviorFunction = std::function<void(entt::entity, entt::entity, entt::registry&, float, SpellLibrary)>;

class BehaviorSystem {
	std::unordered_map<BehaviorType, BehaviorFunction> behaviorMap;
public:
	void initializeBehaviorMap();
	void updateBehavior(entt::registry& registry, float dt, SpellLibrary spellLibrary);
};

