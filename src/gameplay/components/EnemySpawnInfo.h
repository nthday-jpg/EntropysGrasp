#pragma once
#include <vector>
#include "../components/Enemy.h"
#include "../components/MovementComponents.h"

struct EnemySpawnInfo 
{
    EnemyType type;
	// Appearance of the enemy
    std::vector<Position> positions;
    float interval;
};