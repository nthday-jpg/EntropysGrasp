#pragma once
#include <string>
#include <entt/entt.hpp>
#include "../components/lookingDirection.h"

enum class SpellBehaviorType {
    Straight,
    Homing,
    Orbit
};

enum class SpellEffect {
    Burn,
	Freeze,
    Heal,
    Shield,
    Teleport,
    SummonCreature,
	Poison,
};

struct SpellData {
    float damage;
    float manaCost;
    float castTime;
    float cooldowns;
    float speed;
    float size;
    float duration;
    float radius;
    SpellEffect effect;
    SpellBehaviorType behaviorType;
};


struct OnActivateSpell {
    std::string spellID;
    OnActivateSpell(std::string s) : spellID(s) {}
};

struct SpellID {
	std::string spellID;
};