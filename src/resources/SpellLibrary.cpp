#include "SpellLibrary.h"
#include <fstream>
#include <stdexcept>

SpellData SpellLibrary::getSpell(SpellID spellName) const {
	auto it = spellDatabase.find(spellName);
	if (it != spellDatabase.end()) {
		return it->second;
	} else {
		throw std::runtime_error("Spell not found in the library.");
	}
}

bool SpellLibrary::loadSpells(const std::string& filePath) {
	// Load spells from a file or other source
	// This is a placeholder for actual loading logic
	// For example, you might read from a JSON or XML file and populate spellDatabase
	return true;
}

SpellID SpellLibrary::stringtoSpellName(std::string str) const {
	if (str == "Fireball") return SpellID::Fireball;
	if (str == "IceSpike") return SpellID::IceSpike;
	if (str == "Heal") return SpellID::Heal;
	if (str == "Shield") return SpellID::Shield;
	if (str == "Teleport") return SpellID::Teleport;
	if (str == "SummonCreature") return SpellID::SummonCreature;
	if (str == "PoisonCloud") return SpellID::PoisonCloud;
	if (str == "PenetratingShot") return SpellID::PenetratingShot;
	if (str == "KnockbackBlast") return SpellID::KnockbackBlast;
	if (str == "ExplosionWave") return SpellID::ExplosionWave;
	throw std::runtime_error("Unknown spell name: " + str);
}