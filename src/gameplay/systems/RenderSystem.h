#pragma once
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class RenderSystem {
	entt::registry& registry; // Pointer to the registry containing entities
	RenderSystem(entt::registry& registry) : registry(registry) {}
public:
    void render(sf::RenderTarget& target);
};

//RenderSystem renderSystem;
// ...
// window.clear();
// renderSystem.render(registry, window);
// window.display();
