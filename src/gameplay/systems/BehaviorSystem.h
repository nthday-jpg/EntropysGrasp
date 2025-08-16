#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include <functional>
#include "../../manager/SpellLibrary.h"
#include "../../manager/EnemyLibrary.h"
#include "../../gameplay/components/Behavior.h"

using BehaviorFunction = std::function<void(entt::entity, entt::entity, entt::registry&, float)>;

class BehaviorSystem 
{
	std::unordered_map<BehaviorType, BehaviorFunction> behaviorMap;

public:
	BehaviorSystem();
	void initializeBehaviorMap();
	void updateBehavior(entt::registry& registry, float dt);
};

