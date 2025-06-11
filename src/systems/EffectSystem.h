#pragma once
#include <entt/entt.hpp>

enum class EffectType {
	Frozen,
	Burning,
	Poisoned,
	Aggressive,
	Expel,
};

class EffectSystem {
public:
	virtual ~EffectSystem() {};
	virtual void apply(entt::registry& registry, float dt) = 0;
	virtual void deactivate(entt::registry& registry, entt::entity entity) = 0;
};

class FrozenEffect : public EffectSystem {
public:
	void apply(entt::registry& registry, float dt) override;
	void deactivate(entt::registry& registry, entt::entity entity) override;
};

class BurningEffect : public EffectSystem {
public:
	void apply(entt::registry& registry, float dt) override;
	void deactivate(entt::registry& registry, entt::entity entity) override;
};

class PoisonedEffect : public EffectSystem {
public:
	void apply(entt::registry& registry, float dt) override;
	void deactivate(entt::registry& registry, entt::entity entity) override;
};

class AggressiveEffect : public EffectSystem {
public:
	void apply(entt::registry& registry, float dt) override;
	void deactivate(entt::registry& registry, entt::entity entity) override;
};

class ExpelEffect : public EffectSystem {
public:
	void apply(entt::registry& registry, float dt) override;
	void deactivate(entt::registry& registry, entt::entity entity) override;
};