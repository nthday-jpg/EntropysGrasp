#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "../components/Tile.h"
#include "../components/MovementComponents.h"
#include <string>
#include <vector>
#include <unordered_map>

class MapManager {
	std::unordered_map<std::string, Map>mapDatabase;
	std::string currentMapName;

	bool loadMap(const std::string& mapFilePath);
	MapManager();
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;
public:
	static MapManager& getInstance();
	Map& getMap(const std::string& mapName);
    void transitionToMap(const std::string& newMapName);
	void update();
};