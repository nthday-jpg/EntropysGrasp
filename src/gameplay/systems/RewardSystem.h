#pragma once
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include "../components/Reward.h"

class RewardSystem
{
	entt::registry& registry;
	entt::dispatcher* dispatcher;

	int totalExperience = 0;
	int totalGold = 0;
public:
	RewardSystem(entt::registry& reg)
		: registry(reg), dispatcher(nullptr) {}

	void sinkEvents();

	// This function is used to write rewards to account
	void giveReward();

private:
	void onEvent(const Reward& reward);
	void increaseExperience(int amount);
	void increaseGold(int amount);
	void giveMana(entt::entity target, float amount);
};