#pragma once
#include <entt/entt.hpp>
#include "../components/statComponent.h"
#include "../components/Entitytag.h"
#include "../components/EffectTag.h"

enum class EffectType {
	Frozen,
	Burning,
	Poisoned,
	Aggressive,
};

void applyEffect(entt::entity entity, entt::registry& registry, EffectType effectType) {
	switch (effectType) {
	case EffectType::Frozen:
		registry.emplace_or_replace<FrozenTag>(entity);
		break;
	case EffectType::Burning:
		registry.emplace_or_replace<BurningTag>(entity);
		break;
	case EffectType::Poisoned:
		registry.emplace_or_replace<PoisonedTag>(entity);
		break;
	case EffectType::Aggressive:
		registry.emplace_or_replace<AggressiveTag>(entity);
		break;
	default:
		break;
	}
}

class EffectSystem {
public:
	virtual ~EffectSystem() {};
	virtual void apply(entt::registry& registry) = 0;
};

class FrozenEffect : public EffectSystem {
public:
	void apply(entt::registry& registry) override 
	{
		auto view = registry.view<FrozenTag, EnemyTag>();
		// speed = 0, disable shooting, etc.
		// call update
	}
};

class BurningEffect : public EffectSystem {
public:
	void apply(entt::registry& registry) override 
	{
		// call update
	}
};

class PoisonedEffect : public EffectSystem {
public:
	void apply(entt::registry& registry) override
	{
		auto view = registry.view<PoisonedTag, EnemyTag>();
		// reduce speed, atk
		// call update
	}
};

class AggressiveEffect : public EffectSystem {
public:
	void apply(entt::registry& registry) override 
	{
		auto check = registry.view<AggressiveTag, EnemyTag>();
		for (auto [entity, aggressiveTag] : check.each()) {
			auto stats = registry.try_get<SpeedComponent, AttackComponent>(entity);
			// Increase speed and attack power
		}
	}
};