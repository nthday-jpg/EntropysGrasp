#include <iostream>
#include <entt/entt.hpp>
#include "src/systems/CollisionSystem.h"
#include "src/components/EntityTag.h"
#include "src/components/hitbox.h"
#include "src/components/movementComponents.h"
#include <SFML/Graphics.hpp>

int main() {
    entt::registry registry;
    entt::dispatcher dispatcher;
    CollisionSystem collisionSystem(registry, dispatcher);

    // Create player entity
    auto player = registry.create();
    registry.emplace<PlayerTag>(player);
    registry.emplace<Position>(player, Position( 0.0f, 0.0f ));
    registry.emplace<Hitbox>(player, Hitbox(10.0f, 10.0f, 0.0f, 0.0f));
    auto enemy = registry.create();
    registry.emplace<EnemyTag>(enemy);
    registry.emplace<Position>(enemy, Position(10.0f, 10.0f));
    registry.emplace<Hitbox>(enemy, Hitbox(10.0f, 10.0f, 10.0f, 10.0f));

    auto wall = registry.create();
    registry.emplace<WallTag>(wall);
    registry.emplace<Position>(wall, Position( 5.0f, 5.0f ));
    registry.emplace<Hitbox>(wall, Hitbox(10.0f, 10.0f, 5.0f, 5.0f));

    auto spell = registry.create();
    registry.emplace<SpellTag>(spell);
    registry.emplace<Position>(spell, Position(10.0f, 5.0f));
    registry.emplace<Hitbox>(spell, Hitbox(10.0f, 10.0f, 10.0f, 5.0f));

    collisionSystem.update();


    return 0;
}