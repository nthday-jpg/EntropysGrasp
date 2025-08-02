#pragma once
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <unordered_map>

class RenderSystem {
	entt::registry& registry; // Pointer to the registry containing entities
public:
	RenderSystem(entt::registry& registry) : registry(registry) {}

	void renderBackGround();
    void render();
	void renderParticles();
	void renderEnemies();
};

//RenderSystem renderSystem;
// ...
// window.clear();
// renderSystem.render(registry, window);
// window.display();
