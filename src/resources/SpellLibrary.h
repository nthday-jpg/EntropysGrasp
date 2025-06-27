#pragma once
#include <unordered_map>
#include <string>
#include <entt/entt.hpp>
#include "../components/Spell.h"
#include "../components/Enemy.h"

class SpellLibrary {
public:
	std::unordered_map<SpellID, SpellData> spellDatabase;
	SpellData getSpell(SpellID spellName) const;
	SpellID stringtoSpellName(std::string str) const;
	bool loadSpells(const std::string& filePath);
};