#include "MapManager.h"
#include <tinyxml2.h>
#include <iostream>
#include "../../manager/TextureManager.h"

using namespace tinyxml2;

MapManager& MapManager::getInstance() {
	static MapManager instance;
	return instance;
}

MapManager::MapManager() {
	// Initialize the map manager, possibly loading default maps or configurations
}

bool MapManager::loadMap() {
    XMLDocument doc;
    std::string mapname = "map";

    if (doc.LoadFile(pathToMaps.c_str()) != XML_SUCCESS) {
        std::cerr << "Error when opening file: " << pathToMaps << std::endl;
        return false;
    }

    // Lấy thẻ <map>
    XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) {
        std::cerr << "Cannot find tag <map>" << std::endl;
        return false;
    }

    // Lấy thông số bản đồ
    int mapWidth = mapElement->IntAttribute("width");
    int mapHeight = mapElement->IntAttribute("height");
    int tileWidth = mapElement->IntAttribute("tilewidth");
    int tileHeight = mapElement->IntAttribute("tileheight");

    std::cout << "Map size: " << mapWidth << "x" << mapHeight
        << " | Tile: " << tileWidth << "x" << tileHeight << std::endl;

    // Khởi tạo bản đồ mới
    Map newMap;
    newMap.width = mapWidth;
    newMap.height = mapHeight;
    newMap.tileSize = sf::Vector2f(tileWidth, tileHeight);

    // Lấy từng <layer>
    XMLElement* layerElement = mapElement->FirstChildElement("layer");
    while (layerElement) {
        XMLElement* dataElement = layerElement->FirstChildElement("data");
        if (!dataElement || std::string(dataElement->Attribute("encoding")) != "csv") {
            std::cerr << "Layer Data not in CSV type" << std::endl;
            return false;
        }

        const char* csvText = dataElement->GetText();
        if (!csvText) {
            std::cerr << "Not have content in CSV" << std::endl;
            return false;
        }

        std::stringstream ss(csvText);
        std::string item;
        std::vector<int> tiles;

        while (std::getline(ss, item, ',')) {
            int gid = std::stoi(item);
            tiles.push_back(gid);
        }

        newMap.layersTileIDs.push_back(std::move(tiles));
        layerElement = layerElement->NextSiblingElement("layer"); // Chuyển sang layer tiếp theo
    }

    // Gán texture cho tile (giả sử đã load từ trước bằng TextureManager)
    newMap.tileTexture = TextureManager::getInstance().getTexture("c");

    // Lưu bản đồ vào mapDatabase
    mapDatabase[mapname] = std::move(newMap);
    currentMapName = mapname;

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