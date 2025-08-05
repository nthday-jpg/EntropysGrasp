#pragma once
#include <entt/entity/entity.hpp>

enum class RewardType
{
	Experience,
	Gold,
	Item,
	Mana,
};

struct Reward
{
	RewardType type;
	float amount;
	entt::entity target; // Entity that receives the reward

	Reward(RewardType type, float amount, entt::entity target)
		: type(type), amount(amount), target(target) {}
};