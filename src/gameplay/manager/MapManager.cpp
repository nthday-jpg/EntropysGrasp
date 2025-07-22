#include "MapManager.h"
#include <tinyxml2.h>
#include <iostream>

using namespace tinyxml2;

MapManager& MapManager::getInstance() {
	static MapManager instance;
	return instance;
}

MapManager::MapManager() {
	// Initialize the map manager, possibly loading default maps or configurations
}

bool MapManager::loadMap(const std::string& mapFilePath) {
    XMLDocument doc;
	std::string mapname = "map";
    if (doc.LoadFile(mapFilePath.c_str()) != XML_SUCCESS) {
        std::cerr << "Lỗi khi mở file: " << mapFilePath << std::endl;
        return false;
    }

    // Lấy thẻ <map>
    XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) {
        std::cerr << "Không tìm thấy thẻ <map>" << std::endl;
        return false;
    }

    // Lấy thông số bản đồ
    int mapWidth = mapElement->IntAttribute("width");
    int mapHeight = mapElement->IntAttribute("height");
    int tileWidth = mapElement->IntAttribute("tilewidth");
    int tileHeight = mapElement->IntAttribute("tileheight");

    std::cout << "Kích thước map: " << mapWidth << "x" << mapHeight
        << " | Tile: " << tileWidth << "x" << tileHeight << std::endl;

    // Lấy layer đầu tiên
    XMLElement* layerElement = mapElement->FirstChildElement("layer");
    if (!layerElement) {
        std::cerr << "Không tìm thấy layer" << std::endl;
        return false;
    }

    XMLElement* dataElement = layerElement->FirstChildElement("data");
    if (!dataElement || std::string(dataElement->Attribute("encoding")) != "csv") {
        std::cerr << "Dữ liệu layer không ở dạng CSV" << std::endl;
        return false;
    }

    const char* csvText = dataElement->GetText();
    if (!csvText) {
        std::cerr << "Không có nội dung CSV" << std::endl;
        return false;
    }

    // Parse CSV thành vector<int>
    std::stringstream ss(csvText);
    std::string item;
    std::vector<int> tiles;

    while (std::getline(ss, item, ',')) {
        int gid = std::stoi(item);
        tiles.push_back(gid);
    }

    // Lưu thông tin
    //this->mapDatabase[mapname].width = mapWidth;
    //this->mapDatabase[mapname].height = mapHeight;
    //this->mapDatabase[mapname].tileSize.x = tileWidth;
    //this->mapDatabase[mapname].tileSize.y = tileHeight;
    //this->tileIDs = std::move(tiles);

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