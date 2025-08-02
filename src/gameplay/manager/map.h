#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class Map {
public:
	float width;
	float height;
	sf::Vector2f tileSize;
	std::vector<int> tileIDs; // Vector to hold tile IDs
	std::vector<std::vector<int>> layersTileIDs;
	entt::registry registry; // Registry for entities in the map
	entt::dispatcher dispatcher; // Dispatcher for events in the map
	sf::Texture* mapBackground;
	sf::Texture* tileTexture;
	void update(float dt);
};