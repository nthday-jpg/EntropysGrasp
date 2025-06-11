#pragma once
#include <entt/entt.hpp>
#include "../components/CollisionType.h"
#include "../components/CollisionEvent.h"
#include "../Utils/SpatialHash.h"

// Collision System detects collisions between entities in a game world base on entity's hitbox.
// 
// It uses spatial hasing to efficiently manage and detect collisions.
// Collisions are detected and stored as CollisionEvent objects in a vector
// for later processing, such as resolving overlaps or triggering game events.
// 
// Collision System also provides methods to check if two entities intersect
// and to resolve physical overlaps between them.



class CollisionSystem {
public:
    CollisionSystem(entt::registry& registry) : registry(registry)
    {
        collisionEvents.reserve(200);

    }

    void detectCollisions();

    // This method pushes two entities that overlaping away each other
    void resolvePhysicalOverlap(entt::entity e1, entt::entity e2);

    const std::vector<CollisionEvent>& getCollisionEvents() const;

private:
    bool isIntersect(entt::entity e1, entt::entity e2) const;
    CollisionType getCollisionType(entt::entity e) const;

    std::vector<CollisionEvent> collisionEvents;
    entt::registry& registry;

    void resolveRR(entt::entity e1, entt::entity e2);
    void resolveCC(entt::entity e1, entt::entity e2);
};