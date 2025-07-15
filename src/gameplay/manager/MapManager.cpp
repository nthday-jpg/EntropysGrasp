#include "MapManager.h"
#include <tinyxml2.h>

MapManager& MapManager::getInstance() {
	static MapManager instance;
	return instance;
}

MapManager::MapManager() {
	// Initialize the map manager, possibly loading default maps or configurations
}

bool MapManager::loadMap(const std::string& mapFilePath) {

	return true;
}

void MapManager::transitionToMap(const std::string& newMapPath) {
	
}

void MapManager::update() {
	// Update logic for the map manager
	// This could include checking for player position, handling transitions, etc.
}

Map& MapManager::getMap(const std::string& mapName) {
	auto it = mapDatabase.find(mapName);
	if (it != mapDatabase.end()) {
		return it->second;
	}
	throw std::runtime_error("Map not found: " + mapName);
}