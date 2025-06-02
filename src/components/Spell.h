#pragma once
#include <string>
#include <entt/entt.hpp>

enum class SpellEffect {
    Burn,
	Freeze,
    Heal,
    Shield,
    Teleport,
    SummonCreature,
	Poison
};

using BehaviorFunction = std::function<void(entt::entity, entt::registry&, float)>;

struct SpellData {
    std::string id;
	float damage;
    float manaCost;
    float castTime;
    float cooldowns;
    float velo;
    float size;
    float duration;
	SpellEffect effect;
	BehaviorFunction updateFunction;
};

std::unordered_map<std::string, SpellData> spellDatabase;

SpellData getSpellData(std::string spellName) {
    return spellDatabase[spellName];
}

struct whatSpell {
    std::string spellid;
};