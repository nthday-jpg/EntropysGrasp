#include "RenderSystem.h"
#include "../components/MovementComponents.h" // or your position component

void RenderSystem::render(entt::registry& registry, sf::RenderTarget& target) {
    auto view = registry.view<Position, sf::Sprite>();
    for (auto [entity, pos, sprite] : view.each()) {
        sprite.setPosition(pos);
        target.draw(sprite);
    }
}