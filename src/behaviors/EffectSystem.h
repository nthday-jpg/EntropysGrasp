#pragma once
#include <entt/entt.hpp>
#include "../components/statComponent.h"
#include "../components/Entitytag.h"
#include "../components/EffectTag.h"

class EffectSystem {
public:
	virtual ~EffectSystem() {};
	virtual void apply(entt::registry& registry) = 0;
	virtual void update(entt::registry& registry, float delta_t) = 0;
	virtual bool isActive() const 
	{
		return true; 
	}
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