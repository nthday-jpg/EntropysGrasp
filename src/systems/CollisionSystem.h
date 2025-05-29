#pragma once
#include <entt/entt.hpp>
#include "../components/hitbox.h"
#include "../components/EntityTag.h"
#include "../Utils/SpatialHash.h"
#include "../components/movementComponents.h"

//include the effect components here

enum class CollisionType {
	Player = 0,
	Enemy = 1,
	Spell = 2,
	Wall = 3,
	None = 4,
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


class CollisionSystem
{
	std::vector<CollisionEvent> collisionEvents;
	SpatialHashGrid spatialHashGrid;
	entt::registry& registry;
	entt::dispatcher& dispatcher;

	//Function to check if two hitboxes intersect
	bool isIntersect(Hitbox& hitbox1, Hitbox& hitbox2);

	//clear every frame
	//check for collision and add them to the list
	void collectCollision();

	//Get the collision type based on the entity tags
	CollisionType getCollisionType(entt::entity e);

	//resolve the collisions in the collisionEvents list
	//trigger the appropriate events
	void resolveCollsions();
public:
	CollisionSystem(entt::registry& registry, entt::dispatcher& dispatcher)
		: registry(registry), dispatcher(dispatcher) {
		collisionEvents.reserve(200);
	}

	void update()
	{
		collectCollision();
		resolveCollsions();
	}

};


