#pragma once
#include <entt/entt.hpp>

enum class EffectType {
	Frozen,
	Burning,
	Poisoned,
	Aggressive,
};

class EffectSystem {
public:
	virtual ~EffectSystem() {};
	virtual void apply(entt::registry& registry) = 0;
};

class FrozenEffect : public EffectSystem {
public:
	void apply(entt::registry& registry) override;
};

class BurningEffect : public EffectSystem {
public:
	void apply(entt::registry& registry) override;
};

class PoisonedEffect : public EffectSystem {
public:
	void apply(entt::registry& registry) override;
};

class AggressiveEffect : public EffectSystem {
public:
	void apply(entt::registry& registry) override;
};