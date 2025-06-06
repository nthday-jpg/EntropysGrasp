#pragma once
#include <entt/entt.hpp>
#include <cmath>
#include "../resources/SpellLibrary.h"
#include "../components/Behavior.h"

using BehaviorFunction = std::function<void(entt::entity, entt::entity, entt::registry&, float, SpellLibrary)>;
std::unordered_map<SpellBehaviorType, BehaviorFunction> behaviorMap;

void initializeBehaviorMap();
entt::entity triggerSpellEffect(entt::registry& registry, entt::entity caster, std::string spellID, SpellLibrary spellLibrary);