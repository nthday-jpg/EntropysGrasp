#pragma once
#include <unordered_map>
#include <string>
#include <entt/entt.hpp>
#include "../gameplay/components/Spell.h"
#include "../gameplay/components/Enemy.h"

class SpellLibrary 
{
	std::string path = "src/resources/spell.json";
public:
	std::unordered_map<SpellID, SpellData> spellDatabase;

	static SpellLibrary& getInstance();
	const SpellData& getSpell(SpellID spellName) const;
private:
	SpellLibrary();
	SpellLibrary(const SpellLibrary&) = delete;
	SpellLibrary& operator=(const SpellLibrary&) = delete;
	bool loadSpells();
};