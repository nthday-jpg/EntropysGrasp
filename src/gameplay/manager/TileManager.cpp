#include "TileManager.h"
#include "../components/StatComponent.h"
#include "../components/EntityTags.h"
#include "../components/MovementComponents.h"
#include "../../manager/TextureManager.h"

constexpr float INF = 100000.0f;

entt::entity TileManager::createTile(entt::registry& registry, TileType tileType, sf::Vector2f position) {
	const sf::IntRect& tileIntRect = getTileIntRect(tileType);

	entt::entity tileEntity = registry.create();
	registry.emplace<TileTag>(tileEntity);
	registry.emplace<Position>(tileEntity, position.x, position.y);
	registry.emplace<TileType>(tileEntity, tileType);
	registry.emplace<sf::IntRect>(tileEntity, tileIntRect);
	if (isCollidableTile(tileType)) {
		registry.emplace<RepelResistance>(tileEntity, INF);
	}
	else {
		registry.emplace<RepelResistance>(tileEntity, 0.0f);
	}
	return tileEntity;
}

bool TileManager::loadTiles(const std::string& tileFilePath) {
	return true;
}

void TileManager::setTileIntRect(TileType type, const sf::IntRect& rect) {
	tileDatabase[type] = rect;
}

const sf::IntRect& TileManager::getTileIntRect(TileType type) const {
	auto it = tileDatabase.find(type);
	if (it != tileDatabase.end()) {
		return it->second;
	}
	throw std::runtime_error("Tile type not found: " + std::to_string(static_cast<int>(type)));
}