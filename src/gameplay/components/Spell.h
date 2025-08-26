#pragma once
#include <string>
#include <entt/entt.hpp>
#include "../components/lookingDirection.h"
#include "Behavior.h"

enum class SpellEffect 
{
    Burn,
	Freeze,
    Heal,
    Shield,
    Teleport,
    SummonCreature,
	Poison,
	Penetrate,
	Knockback,
	Explosion,
	None,
};

struct SpellData 
{
    float damage;
    float manaCost;
    float castTime;
    float cooldowns;
    float speed;
    float duration;
    float radius;
	int count;
	float timeDecreasePerHit;
    SpellEffect effect;
    BehaviorType behaviorType;
};

enum class SpellID 
{
	Fireball,
	IceSpike,
	Heal,
	Shield,
	Teleport,
	SummonCreature,
	PoisonCloud,
	PenetratingShot,
	KnockbackBlast,
	ExplosionWave,
	Unknown
};

inline const std::unordered_map<std::string, SpellID> stringToSpellIDMap = 
{
	{"Fireball", SpellID::Fireball},
	{"IceSpike", SpellID::IceSpike},
	{"Heal", SpellID::Heal},
	{"Shield", SpellID::Shield},
	{"Teleport", SpellID::Teleport},
	{"SummonCreature", SpellID::SummonCreature},
	{"PoisonCloud", SpellID::PoisonCloud},
	{"PenetratingShot", SpellID::PenetratingShot},
	{"KnockbackBlast", SpellID::KnockbackBlast},
	{"ExplosionWave", SpellID::ExplosionWave},
};

inline SpellID stringToSpellID(const std::string& str) 
{
	auto it = stringToSpellIDMap.find(str);
	if (it != stringToSpellIDMap.end())
		return it->second;
	return SpellID::Unknown; // Default case if not found
}

inline std::string spellIDToString(SpellID id) 
{
	for (const auto& pair : stringToSpellIDMap) 
	{
		if (pair.second == id) 
		{
			return pair.first;
		}
	}
	return "Unknown";
}

inline SpellEffect stringToSpellEffect(const std::string& str) 
{
	if (str == "Burn") return SpellEffect::Burn;
	if (str == "Freeze") return SpellEffect::Freeze;
	if (str == "Heal") return SpellEffect::Heal;
	if (str == "Shield") return SpellEffect::Shield;
	if (str == "Teleport") return SpellEffect::Teleport;
	if (str == "SummonCreature") return SpellEffect::SummonCreature;
	if (str == "Poison") return SpellEffect::Poison;
	if (str == "Penetrate") return SpellEffect::Penetrate;
	if (str == "Knockback") return SpellEffect::Knockback;
	if (str == "Explosion") return SpellEffect::Explosion;
	if (str == "None") return SpellEffect::None;
	throw std::runtime_error("Unknown SpellEffect: " + str);
}