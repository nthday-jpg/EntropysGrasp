#include "EnemyLibrary.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

bool EnemyLibrary::loadFromFile() {
    std::cout << "Loading enemy data from: " << path << std::endl;
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open enemy.json" << std::endl;
        return false;
    }

    json enemy;
    try {
        file >> enemy;

        for (const auto& [name, enemy] : enemy.items()) {
            EnemyType type = stringToEnemyType(name);

            if (type == EnemyType::Unknown) {
                std::cerr << "Unknown enemy type: " << name << std::endl;
                continue;
            }

            if (!enemy.contains("stats")) {
                std::cerr << "Missing 'stats' section for enemy: " << name << std::endl;
                continue;
            }
            if (!enemy.contains("reward")) {
                std::cerr << "Missing 'reward' section for enemy: " << name << std::endl;
                continue;
			}

            const auto& stats = enemy["stats"];

            EnemyData data;
            data.health.max = data.health.current = stats.value("health", 100.0f);
            data.attack.value = stats.value("attack", 10.0f);
            data.mana.value = stats.value("mana", 50.0f);
            data.resistance.value = stats.value("resistance", 5.0f);
            data.speed.value = stats.value("speed", 2.0f);
            
            const auto& reward = enemy["reward"];
            data.exp = reward.value("experience", 100.f);
            data.gold = reward.value("gold", 100.f);

            enemyDatabase[type] = data;

        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing enemy.json: " << e.what() << std::endl;
        return false;
    }

    file.close();
    return true;
}

EnemyLibrary::EnemyLibrary()
{
	if (!loadFromFile())
	{
		throw std::runtime_error("Failed to load enemy data from file.");
	}
}

const EnemyData& EnemyLibrary::getEnemyData(EnemyType type) const 
{
	auto it = enemyDatabase.find(type);
	if (it != enemyDatabase.end())
	{
		return it->second;
	}
	else
	{
		throw std::runtime_error("EnemyType not found: " + std::to_string(static_cast<int>(type)));
	}
}

EnemyLibrary& EnemyLibrary::getInstance() 
{
	static EnemyLibrary instance;
	return instance;
}