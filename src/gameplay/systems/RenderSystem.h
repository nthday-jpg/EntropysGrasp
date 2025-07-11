#pragma once
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class RenderSystem {
public:
    void render(entt::registry& registry, sf::RenderTarget& target);
};

//RenderSystem renderSystem;
// ...
// window.clear();
// renderSystem.render(registry, window);
// window.display();
