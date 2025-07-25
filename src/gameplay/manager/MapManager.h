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
	std::string pathToMaps = "assets/map/full.tmx";
	std::unordered_map<std::string, Map>mapDatabase;

	MapManager();
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;
public:
	std::string currentMapName = "map";
	bool loadMap();
	static MapManager& getInstance();
	Map& getMap(const std::string& mapName);
    void transitionToMap(const std::string& newMapName);
	void update();
};