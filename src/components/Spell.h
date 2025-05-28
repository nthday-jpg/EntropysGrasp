#pragma once
#include <string>
#include <entt/entt.hpp>

struct SpellData {
    std::string id;
    float manaCost;
    float castTime;
    float cooldowns;
    float velo;
    float size;
    float duration;
    std::string effectId;
};

std::unordered_map<std::string, SpellData> spellDatabase;

SpellData getSpellData(std::string spellName) {
    return spellDatabase[spellName];
}