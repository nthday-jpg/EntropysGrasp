#include "RenderSystem.h"
#include "../components/MovementComponents.h" // or your position component
#include "../../manager/WindowManager.h"
#include <iostream>
#include "../components/Particle.h"

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