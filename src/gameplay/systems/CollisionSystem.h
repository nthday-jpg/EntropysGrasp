#pragma once
#include <entt/entt.hpp>
#include "../components/CollisionType.h"
#include "../components/CollisionEvent.h"
#include "../../utils/SpatialHash.h"

// Collision System detects collisions between entities in a game world base on entity's hitbox.
// 
// It uses spatial hasing to efficiently manage and detect collisions.
// Collisions are detected and stored as CollisionEvent objects in a vector
// for later processing, such as resolving overlaps or triggering game events.
// 
// Collision System also provides methods to check if two entities intersect
// and to resolve physical overlaps between them.



class CollisionSystem 
{
    std::vector<CollisionEvent> collisionEvents;
    entt::registry& registry;
	entt::dispatcher* dispatcher; // Optional dispatcher for event handling
public:
    CollisionSystem(entt::registry& registry, entt::dispatcher* dispatcher);
    void detectCollisions();

	void update(float dt);
private:
    const std::vector<CollisionEvent>& getCollisionEvents() const;
    void resolvePhysicalOverlap(entt::entity e1, entt::entity e2);
    bool isIntersect(entt::entity e1, entt::entity e2) const;
    CollisionType getCollisionType(entt::entity e) const;
    void resolveRR(entt::entity e1, entt::entity e2);
    void resolveCC(entt::entity e1, entt::entity e2);
};