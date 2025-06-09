#pragma once
#include "../components/statComponent.h"
#include "../components/behavior.h"
//include behavior here too

enum class EnemyType
{
	Goblin,
	Troll,
	Dragon,
	Skeleton
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