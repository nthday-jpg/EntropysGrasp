#pragma once
#include "../components/Enemy.h"
#include <unordered_map>
#include <string>

class EnemyLibrary {
    std::unordered_map<EnemyType, EnemyData> enemyDataMap;
    std::string path = "../src/assets/enemy.json";

public:
    EnemyLibrary();

    const EnemyData& getEnemyData(EnemyType type) const;

private:
	// not implemented
    bool loadFromFile();

};