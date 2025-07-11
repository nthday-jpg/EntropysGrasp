#pragma once
#include "../gameplay/components/Enemy.h"
#include <unordered_map>
#include <string>

class EnemyLibrary {
    std::unordered_map<EnemyType, EnemyData> enemyDatabase;
    std::string path = "src/resources/enemy.json";

    EnemyLibrary();
    EnemyLibrary(const EnemyLibrary&) = delete;
    EnemyLibrary& operator=(const EnemyLibrary&) = delete;

public:
    static EnemyLibrary& getInstance();

    const EnemyData& getEnemyData(EnemyType type) const;

private:
    // not implemented
    bool loadFromFile();

};