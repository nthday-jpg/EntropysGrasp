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
	//ifstream file(path);
	//if (!file.is_open()) 
	//{
	//	cerr << "Failed to open spells.json" << endl;
	//	return false;
	//}
	//json spells;
	//file >> spells;
	//try 
	//{
	//	for (const auto& [name, stat] : spells.items()) 
	//  {
	//		SpellID spellID = stringToSpellID(name);
	//		SpellData data;
	//		data.damage = stat.value("damage", 1.0f);
	//		data.manaCost = stat.value("manaCost", 1.0f);
	//		data.castTime = stat.value("castTime", 1.0f);
	//		data.cooldowns = stat.value("cooldowns", 1.0f);
	//		data.speed = stat.value("speed", 1.0f);
	//		data.size = stat.value("size", 1.0f);
	//		data.duration = stat.value("duration", 1.0f);
	//		data.radius = stat.value("radius", 1.0f);
	//		string effectStr = stat.value("effect", "None");
	//		data.effect = stringToSpellEffect(effectStr);
	//		string behaviorStr = stat.value("behaviorType", "HomingEnemy");
	//		data.behaviorType = stringToBehaviorType(behaviorStr);
	//		spellDatabase[spellID] = data;
	//	}
	//} 
	//catch (const std::exception& e) 
	//{
	//	cerr << "Error parsing spells.json: " << e.what() << endl;
	//	return false;
	//}
	//file.close();
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