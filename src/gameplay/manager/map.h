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
	entt::registry registry; // Registry for entities in the map
	entt::dispatcher dispatcher; // Dispatcher for events in the map
	std::shared_ptr<sf::Texture> mapBackground;
	void update(float dt);
};