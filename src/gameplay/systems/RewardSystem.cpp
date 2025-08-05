#include "RewardSystem.h"
#include "../components/statComponent.h"
#include <iostream>

void RewardSystem::sinkEvents()
{
	if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) 
	{
		(*dispatcher)->sink<Reward>().connect<&RewardSystem::onEvent>(*this);
	}
}

void RewardSystem::onEvent(const Reward& reward)
{
	switch (reward.type)
	{
	case RewardType::Experience:
		increaseExperience(reward.amount);
		break;
	case RewardType::Gold:
		increaseGold(reward.amount);
		break;
	case RewardType::Item:
		// Handle item reward logic here
		break;
	case RewardType::Mana:
		giveMana(reward.target, reward.amount);
		break;
	}
}

void RewardSystem::increaseExperience(int amount)
{
	totalExperience += amount;
}

void RewardSystem::increaseGold(int amount)
{
	totalGold += amount;
}

void RewardSystem::giveMana(entt::entity target, int amount)
{
	if (registry.valid(target))
	{
		if (Mana* mana = registry.try_get<Mana>(target))
		{
			mana->value += amount;
		}
	}
}

void RewardSystem::giveReward()
{

}