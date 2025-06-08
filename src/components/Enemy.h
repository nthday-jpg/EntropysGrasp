#pragma once
#include "../components/statComponent.h"
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
	// Behavior behavior; // Placeholder for behavior component
};