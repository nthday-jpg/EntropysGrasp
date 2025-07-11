#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "../components/tile.h"

class TileManager {
	std::unordered_map<TileType, sf::IntRect> tileDatabase;
	bool loadTiles(const std::string& tileFilePath);
	void setTileIntRect(TileType type, const sf::IntRect& rect);
	const sf::IntRect& getTileIntRect(TileType type) const;
	entt::entity createTile(entt::registry& registry, TileType tileType, sf::Vector2f position);
};