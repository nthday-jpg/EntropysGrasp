#pragma once
#include <entt/entt.hpp>

enum class EffectType 
{
	Frozen,
	Burning,
	Poisoned,
	Aggressive,
	Repel,
};

class EffectSystem 
{
protected:
	entt::registry& registry;
public:
	EffectSystem(entt::registry& registry) 
		: registry(registry) {}	
	virtual ~EffectSystem() {};
	void applyEffect(entt::entity entity, EffectType effectType, float dt);
	virtual void apply(float dt) = 0;
	virtual void deactivate(entt::entity entity) = 0;
};

class FrozenEffect : public EffectSystem 
{
public:
	FrozenEffect(entt::registry& registry)
		: EffectSystem(registry) {}
	void apply(float dt) override;
	void deactivate(entt::entity entity) override;
};

class BurningEffect : public EffectSystem 
{
public:
	BurningEffect(entt::registry& registry) 
		: EffectSystem(registry) {}
	void apply(float dt) override;
	void deactivate(entt::entity entity) override;
};

class PoisonedEffect : public EffectSystem 
{
public:
	PoisonedEffect(entt::registry& registry) 
		: EffectSystem(registry) {}
	void apply(float dt) override;
	void deactivate(entt::entity entity) override;
};

class AggressiveEffect : public EffectSystem 
{
public:
	AggressiveEffect(entt::registry& registry) 
		: EffectSystem(registry) {}
	void apply(float dt) override;
	void deactivate(entt::entity entity) override;
};

class RepelEffect : public EffectSystem 
{
public:
	RepelEffect(entt::registry& registry)
		: EffectSystem(registry) {}
	void apply(float dt) override;
	void deactivate(entt::entity entity) override;
};