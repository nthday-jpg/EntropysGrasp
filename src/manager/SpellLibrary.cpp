#include "SpellLibrary.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

const SpellData& SpellLibrary::getSpell(SpellID spellName) const 
{
	auto it = spellDatabase.find(spellName);
	if (it != spellDatabase.end()) 
	{
		return it->second;
	} 
	else 
	{
		throw std::runtime_error("Spell not found in the library.");
	}
}

bool SpellLibrary::loadSpells() 
{
	std::cout << "Loading spell data from: " << path << std::endl;
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Failed to open spell.json" << std::endl;
		return false;
	}

	json spell;
	try {
		file >> spell;

		for (const auto& [name, spell] : spell.items()) {
			SpellID spellID = stringToSpellID(name);

			if (spellID == SpellID::Unknown) {
				std::cerr << "Unknown spell spellID: " << name << std::endl;
				continue;
			}

			if (!spell.contains("stats")) {
				std::cerr << "Missing 'stats' section for spell: " << name << std::endl;
				continue;
			}

			const auto& stats = spell["stats"];

			SpellData data;
			data.damage = stats.value("damage", 1.0f);
			data.manaCost = stats.value("manaCost", 1.0f);
			data.castTime = stats.value("castTime", 1.0f);
			data.cooldowns = stats.value("cooldowns", 1.0f);
			data.speed = stats.value("speed", 1.0f);
			data.size = stats.value("size", 1.0f);
			data.duration = stats.value("duration", 1.0f);
			data.radius = stats.value("radius", 1.0f);
			string effectStr = stats.value("effect", "None");
			data.effect = stringToSpellEffect(effectStr);
			string behaviorStr = stats.value("behaviorType", "Orbit");
			data.behaviorType = stringToBehaviorType(behaviorStr);
			data.count = stats.value("count", 1);

			spellDatabase[spellID] = data;

		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing spell.json: " << e.what() << std::endl;
		return false;
	}

	file.close();
	return true;
}

SpellLibrary::SpellLibrary()
{
	if (!loadSpells()) 
	{
		throw std::runtime_error("Failed to load spell data from file.");
	}
}

SpellLibrary& SpellLibrary::getInstance() 
{
	static SpellLibrary instance;
	return instance;
}