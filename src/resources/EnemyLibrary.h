#pragma once
#include "../components/Enemy.h"
#include <unordered_map>
#include <string>

class EnemyLibrary {
public:
	//not implemented yet
    bool loadFromFile(const std::string& filename);
    const EnemyData& getEnemyData(EnemyType type) const;
    std::unordered_map<EnemyType, EnemyData> enemyDatabase;

private:

    // Helper to convert string to EnemyType
    EnemyType stringToEnemyType(const std::string& str);
};