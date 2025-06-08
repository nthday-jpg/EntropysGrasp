#include <entt/entt.hpp>

struct FrozenTag
{
	float remainingTime = 0.0f;
};

struct BurningTag
{
	float remainingTime = 0.0f;
};

struct PoisonedTag
{
	float remainingTime = 0.0f;
};

struct AggressiveTag
{
	float threshold = 10.0f;
};