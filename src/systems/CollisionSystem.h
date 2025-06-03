#pragma once
#include <entt/entt.hpp>
#include "../Utils/SpatialHash.h"

enum class CollisionType {
    Player,
    Enemy,
    Spell,
    Wall,
    None
};

struct CollisionEvent {
    entt::entity entity1;
    entt::entity entity2;
    CollisionType type1;
    CollisionType type2;

    CollisionEvent(entt::entity e1, entt::entity e2, CollisionType t1, CollisionType t2)
        : entity1(e1), entity2(e2), type1(t1), type2(t2) {
    }
};

class CollisionSystem {
public:
    CollisionSystem(entt::registry& registry) : registry(registry)
    {
        collisionEvents.reserve(200);

    }

    void detectCollisions();
    void resolvePhysicalOverlap(entt::entity e1, entt::entity e2);

    const std::vector<CollisionEvent>& getCollisionEvents() const;

private:
    bool isIntersect(entt::entity e1, entt::entity e2) const;
    CollisionType getCollisionType(entt::entity e) const;

    std::vector<CollisionEvent> collisionEvents;
    entt::registry& registry;
};
