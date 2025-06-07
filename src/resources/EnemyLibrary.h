#pragma once
#include "../components/Enemy.h"
#include <unordered_map>
#include <string>

class EnemyLibrary {
public:
    bool loadFromFile(const std::string& filename);
    const EnemyData& getEnemyData(EnemyType type) const;

private:
    std::unordered_map<EnemyType, EnemyData> enemyDataMap;
};