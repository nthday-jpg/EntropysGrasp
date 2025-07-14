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
    for (auto & pair : sprites) {
        sf::Sprite* sprite = pair.second;
        // Check if the entity has a Position component
		WindowManager::getInstance().draw(*sprite);
		std::cout << "render" << std::endl;
	}
}

void RenderSystem::renderParticles() {
    // Implement particle rendering logic here if needed
    // This could involve iterating over a particle component and drawing each particle
    // For now, this is left empty as a placeholder
}