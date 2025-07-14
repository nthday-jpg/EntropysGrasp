#pragma once
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <unordered_map>

class RenderSystem {
	entt::registry& registry; // Pointer to the registry containing entities
	std::unordered_map<entt::entity, sf::Sprite*> sprites; // Cache for sprites to avoid repeated lookups
public:
	RenderSystem(entt::registry& registry) : registry(registry) {}

	void setSprite(entt::entity entity, sf::Sprite* sprite) {
		sprites[entity] = sprite;
	}
	void renderBackGround(sf::Texture* background);
    void render();
	void renderParticles();
};

//RenderSystem renderSystem;
// ...
// window.clear();
// renderSystem.render(registry, window);
// window.display();
