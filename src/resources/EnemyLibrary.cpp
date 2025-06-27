#include "EnemyLibrary.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

bool EnemyLibrary::loadFromFile() {
	ifstream file(path);

	if (!file.is_open()) {
		cerr << "Failed to open enemy.json" << std::endl;
		return false;
	}

	json enemies;
	file >> enemies;

	try {
		for (const auto& [name, stat] : enemies.items()) {
			EnemyType type = stringToEnemyType(name);
			EnemyData data;
			data.health.max = data.health.current = stat.value("health", 100.0f);
			data.attack.value = stat.value("attack", 10.0f);
			data.mana.value = stat.value("mana", 50.0f);
			data.resistance.value = stat.value("resistance", 5.0f);
			data.speed.value = stat.value("weakness", 2.0f);

			enemyDataMap[type] = data;
			// load animation data
		}
	}
	catch (const std::exception& e) {
		cerr << "Error parsing enemy.json: " << e.what() << std::endl;
		return false;
	}
	file.close();
	return true;
}

EnemyLibrary::EnemyLibrary()
{
	if (!loadFromFile()) {
		throw std::runtime_error("Failed to load enemy data from file.");
	}
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
