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
	float duration;
public:
	FrozenEffect(float duration) : duration(duration) {}
	void apply(entt::registry& registry) override 
	{
		auto view = registry.view<FrozenTag, EnemyTag>();
		// speed = 0, disable shooting, etc.
		// call update
	}
	void update(entt::registry& registry, float delta_t) override 
	{
		// Decrease duration and check if `it still active
	}
	bool isActive() const override { return duration > 0; }
};

class BurningEffect : public EffectSystem {
	float duration;
public:
	BurningEffect(float duration) : duration(duration) {}
	void apply(entt::registry& registry) override 
	{
		// call update
	}
	void update(entt::registry& registry, float delta_t) override
	{
		auto view = registry.view<BurningTag, EnemyTag>();
		// Decrease duration and check if it still active
		// decrease health per delta_t
	}
	bool isActive() const override { return duration > 0; }
};

class PoisonedEffect : public EffectSystem {
	float duration;
public:
	PoisonedEffect(float duration) : duration(duration) {}
	void apply(entt::registry& registry) override
	{
		auto view = registry.view<PoisonedTag, EnemyTag>();
		// reduce speed, atk
		// call update
	}
	void update(entt::registry& registry, float delta_t) override 
	{
		// Decrease duration and check if it still active
		// reduce health per delta_t
	}
	bool isActive() const override { return duration > 0; }
};

class AgressiveEffect : public EffectSystem {
	float threshold;
public:
	void apply(entt::registry& registry) override 
	{
		auto check = registry.view<AggressiveTag, EnemyTag>();
		for (auto [entity] : check.each()) {
			auto stats = registry.try_get<SpeedComponent, AttackComponent>(entity);
			// Increase speed and attack power
		}
	}
};