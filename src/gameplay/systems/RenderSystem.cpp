#include "RenderSystem.h"
#include "../components/MovementComponents.h" // or your position component
#include "../../manager/WindowManager.h"
#include "../components/EntityTags.h"
#include <iostream>
#include "../components/Particle.h"
#include "../../manager/TextureManager.h"
#include "../../gameplay/manager/MapManager.h"

void RenderSystem::renderBackGround() {
    const std::string& currentMapName = MapManager::getInstance().currentMapName;
    Map& map = MapManager::getInstance().getMap(currentMapName);

    const int mapWidth = static_cast<int>(map.width);
    const int mapHeight = static_cast<int>(map.height);
    const int tileWidth = static_cast<int>(map.tileSize.x);
    const int tileHeight = static_cast<int>(map.tileSize.y);

    if (!map.tileTexture) {
        std::cerr << "Chưa có texture cho map!" << std::endl;
        return;
    }

    const int tilesetColumns = map.tileTexture->getSize().x / tileWidth;

    // Vẽ từng layer (theo thứ tự)
    for (const auto& layer : map.layersTileIDs) {
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int index = y * mapWidth + x;
                if (index >= layer.size()) continue; // phòng ngừa lỗi index out-of-bound

                int tileID = layer[index];
                if (tileID == 0) continue; // tile rỗng

                int tileIndex = tileID - 1;
                int tu = tileIndex % tilesetColumns;
                int tv = tileIndex / tilesetColumns;

                sf::Sprite sprite(*map.tileTexture);
                sprite.setTextureRect(sf::IntRect(
                    { tu * tileWidth, tv * tileHeight },
                    { tileWidth, tileHeight }
                ));
                sprite.setPosition({ static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight) });

                WindowManager::getInstance().draw(sprite);
            }
        }
    }
}

void RenderSystem::render() {
    renderBackGround();
	renderEnemies();
    auto group = registry.group<Position, sf::Sprite>();

    std::vector<entt::entity> entities;
    entities.reserve(group.size());

    for (auto entity : group) {
        entities.push_back(entity);
    }

    std::sort(entities.begin(), entities.end(), [&group](entt::entity a, entt::entity b) {
        const Position& posA = group.get<Position>(a);
        const Position& posB = group.get<Position>(b);
        return posA.y < posB.y;
    });
    
    for (auto entity : entities) {
        const Position& pos = group.get<Position>(entity);
        sf::Sprite& sprite = group.get<sf::Sprite>(entity);
        sprite.setPosition(pos);
        WindowManager::getInstance().draw(sprite);
    }

    renderParticles();
}

void RenderSystem::renderParticles() {
    auto view = registry.view<ParticleComponent, Position>();
	// Use vertex array for efficient rendering
    for (auto [entity, particle, position] : view.each()) {

        if (registry.any_of<InactiveParticle>(entity)) {
            continue;
        }

        sf::VertexArray* particleArray;
        if (auto* existing = registry.try_get<sf::VertexArray>(entity)) {
            particleArray = existing;
        }
        else {
            // Only create if it doesn't exist
            particleArray = &registry.emplace<sf::VertexArray>(entity, sf::PrimitiveType::TriangleStrip, 4);
        }

        float size = particle.beginSize - (particle.beginSize - particle.endSize) * particle.age/particle.lifetime;

        // Update vertex positions and colors
        (*particleArray)[0].position = sf::Vector2f(position.x - size, position.y - size);
        (*particleArray)[1].position = sf::Vector2f(position.x - size, position.y + size);
        (*particleArray)[2].position = sf::Vector2f(position.x + size, position.y - size);
        (*particleArray)[3].position = sf::Vector2f(position.x + size, position.y + size);

        // Update colors
        (*particleArray)[0].color = particle.colorStart;
        (*particleArray)[1].color = particle.colorStart;
        (*particleArray)[2].color = particle.colorStart;
        (*particleArray)[3].color = particle.colorStart;

		// Draw the particle
        WindowManager::getInstance().draw(*particleArray);
        // Optionally, remove the particle if it has aged out
        
    }
}

void RenderSystem::renderEnemies() {
	auto view = registry.view<EnemyTag, SpellTag>();
    for (auto [entity] : view.each()) {
        if (!registry.all_of<sf::Sprite>(entity)) {
			sf::Texture* texture = TextureManager::getInstance().getTexture("test");
			sf::IntRect rect({0, 0}, {32, 48});
			sf::Sprite sprite(*texture);
			sprite.setTextureRect(rect);
			registry.emplace<sf::Sprite>(entity, sprite);
		}
	}
}