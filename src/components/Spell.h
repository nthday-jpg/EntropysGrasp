#pragma once
#include <string>
#include <entt/entt.hpp>
#include "../components/lookingDirection.h"

enum class SpellEffect {
    Burn,
	Freeze,
    Heal,
    Shield,
    Teleport,
    SummonCreature,
	Poison
};

enum class SpellBehaviorType {
	StraightMovement,
	HomingMovement,
	OrbitMovement
};

struct SpellData {
    std::string id;
	float damage;
    float manaCost;
    float castTime;
    float cooldowns;
    float speed;
    float size;
    float duration;
	SpellEffect effect;
	SpellBehaviorType behaviorType;
};

using BehaviorFunction = std::function<void(entt::entity, entt::registry&, const SpellData&)>;
std::unordered_map<SpellBehaviorType, BehaviorFunction> behaviorMap;

std::unordered_map<std::string, SpellData> spellDatabase;

SpellData getSpellData(std::string spellName) {
    return spellDatabase[spellName];
}

struct OnActivateSpell {
    OnActivateSpell(std::string s) : spellid(s) {}
    std::string spellid;
};