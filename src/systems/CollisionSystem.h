#pragma once
#include <entt/entt.hpp>
#include "../components/hitbox.h"
#include "../Utils/SpatialHash.h"
#include "../components/movementComponents.h"

//include the effect components here

struct CollisionEvent
{
	entt::entity entity1;
	entt::entity entity2;

	CollisionEvent() : entity1(entt::null), entity2(entt::null)
	{
	}

	CollisionEvent(entt::entity e1, entt::entity e2)
		: entity1(e1), entity2(e2)
	{
	}

};

class CollisionSystem
{
	std::vector<CollisionEvent> collisionEvents;
	SpatialHashGrid spatialHashGrid;


	//Function to check if two hitboxes intersect
	bool isIntersect(Hitbox& hitbox1, Hitbox& hitbox2);

	//clear every frame
	//check for collision and add them to the list
	void updateEvents(entt::registry& registry);
public:
	CollisionSystem() {
		collisionEvents.reserve(200);
	}

	//Classify the type of collision then mark the entities for collision
	void classify(entt::registry registry);

	//return the reference to the list of collision events
	std::vector<CollisionEvent>& access();
};


//Move those things below to effect components
struct DamageTag
{
	entt::entity source = entt::null;
};

enum class ExpelSourceType 
{
	Wall,
	Player,
	Magic,
	Enemy,
};

struct ExpelTag 
{
	ExpelSourceType sourceType;         
	entt::entity source = entt::null;  
	sf::Vector2f direction;             
	float strength = 0.0f;           

	ExpelTag(ExpelSourceType srcType, sf::Vector2f dir, float str, entt::entity src = entt::null)
		: sourceType(srcType), source(src), direction(dir), strength(str) {
	}
};
