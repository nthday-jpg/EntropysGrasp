#pragma once
#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>

struct FrozenTag
{
	float originalSpeed;
	float remainingTime;
	bool applied = false;
};

struct BurningTag
{
	float remainingTime;
	bool applied = false;
};

struct PoisonedTag
{
	float originalSpeed;
	float originalAttack;
	float remainingTime;
	bool applied = false;
};

struct AggressiveTag
{
	float originalSpeed;
	float originalAttack;
	float threshold;
	bool applied = false;
};

struct RepelTag
{
	float remainingTime;
	sf::Vector2f force;
};