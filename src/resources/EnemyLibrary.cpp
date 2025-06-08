#include "EnemyLibrary.h"
#include <fstream>
#include <stdexcept>

EnemyType EnemyLibrary::stringToEnemyType(const std::string& str) {
    if (str == "Goblin") return EnemyType::Goblin;
    if (str == "Troll") return EnemyType::Troll;
    if (str == "Dragon") return EnemyType::Dragon;
    if (str == "Skeleton") return EnemyType::Skeleton;
    throw std::runtime_error("Unknown EnemyType: " + str);
}

bool EnemyLibrary::loadFromFile(const std::string& filename) {
	return true;
}

const EnemyData& EnemyLibrary::getEnemyData(EnemyType type) const {
	auto it = enemyDataMap.find(type);
	if (it != enemyDataMap.end()) 
	{
		return it->second;
	}
	else
	{
		throw std::runtime_error("EnemyType not found: " + std::to_string(static_cast<int>(type)));
	}
}

