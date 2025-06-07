#pragma once
#include <unordered_map>
#include <string>
#include <entt/entt.hpp>
#include "../components/Spell.h"

class SpellLibrary {
	std::unordered_map<SpellID, SpellData> spellDatabase;
public:
	SpellData getSpell(SpellID spellName) const {
		return spellDatabase.at(spellName);
	}
	void loadSpells(const std::string& filePath) {
		// Load spells from a file or database
		// This is a placeholder for actual loading logic
	}
};