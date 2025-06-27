#pragma once
#include "../components/statComponent.h"
#include "../components/behavior.h"
//include behavior here too

enum class EnemyType
{
	Goblin,
	Troll,
	Dragon,
	Skeleton,
	Unknown,
};

struct EnemyData
{
	Health health;
	Mana mana;
	Speed speed;
	Attack attack;
	Resistance resistance;
	BehaviorType behaviorType; // Assuming BehaviorType is an enum defined elsewhere
	// Behavior behavior; // Placeholder for behavior component
};

// String to enum map
inline const std::unordered_map<std::string, EnemyType> stringToEnemyTypeMap = {
	{"Goblin", EnemyType::Goblin},
	{"Troll", EnemyType::Troll},
	{"Dragon", EnemyType::Dragon},
	{"Skeleton", EnemyType::Skeleton},
	// ... add more as needed
};

// Helper functions
inline EnemyType stringToEnemyType(const std::string& str) {
	auto it = stringToEnemyTypeMap.find(str);
	if (it != stringToEnemyTypeMap.end())
		return it->second;
	return EnemyType::Unknown; // Default case if not found
}