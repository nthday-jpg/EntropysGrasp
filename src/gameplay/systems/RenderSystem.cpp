#include "RenderSystem.h"
#include "../components/MovementComponents.h" // or your position component
#include "../../manager/WindowManager.h"
#include <iostream>

void RenderSystem::renderBackGround(sf::Texture* background) {
    if (background) {
        sf::Sprite backgroundSprite(*background);
        WindowManager::getInstance().draw(backgroundSprite);
    }
    
    // Optionally, you can clear the sprites map if needed
	// sprites.clear();
}

void RenderSystem::render() {
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
}

void RenderSystem::renderParticles() {
    // Implement particle rendering logic here if needed
    // This could involve iterating over a particle component and drawing each particle
    // For now, this is left empty as a placeholder
}