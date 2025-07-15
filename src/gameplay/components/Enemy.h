#pragma once
#include "../components/StatComponent.h"
#include "../components/Behavior.h"

//include behavior here too

enum class EnemyType
{
	Goblin,
	Troll,
	Dragon,
	Skeleton,
	Orc,
	Unknown,
};

struct EnemyData
{
	Health health;
	Mana mana;
	Speed speed;
	Attack attack;
	RepelResistance resistance;
	BehaviorType behaviorType = BehaviorType::HomingPlayer; // Assuming BehaviorType is an enum defined elsewhere
	// Behavior behavior; // Placeholder for behavior component
};

// String to enum map
inline const std::unordered_map<std::string, EnemyType> stringToEnemyTypeMap = 
{
	{"Goblin", EnemyType::Goblin},
	{"Troll", EnemyType::Troll},
	{"Dragon", EnemyType::Dragon},
	{"Skeleton", EnemyType::Skeleton},
	{"Orc", EnemyType::Orc},
	// ... add more as needed
};

// Helper functions
inline EnemyType stringToEnemyType(const std::string& str) 
{
	auto it = stringToEnemyTypeMap.find(str);
	if (it != stringToEnemyTypeMap.end())
	{
		return it->second;
	}
	return EnemyType::Unknown;
}