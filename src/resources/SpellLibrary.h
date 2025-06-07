#pragma once
#include <unordered_map>
#include <string>
#include <entt/entt.hpp>
#include "../components/Spell.h"

class SpellLibrary {
	std::unordered_map<std::string, SpellData> spellDatabase;
public:
	SpellData getSpell(const std::string& spellName) const {
		return spellDatabase.at(spellName);
	}
	void loadSpells(const std::string& filePath) {
		// Load spells from a file or database
		// This is a placeholder for actual loading logic
	}
};