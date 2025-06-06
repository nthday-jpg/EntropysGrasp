#include "EffectSystem.h"
#include "../components/statComponent.h"
#include "../components/Entitytag.h"
#include "../components/EffectTag.h"
#include "../components/movementComponents.h"

void applyEffect(entt::entity entity, entt::registry& registry, EffectType effectType) {
	switch (effectType) {
		case EffectType::Frozen: {
			registry.emplace_or_replace<FrozenTag>(entity);
			FrozenEffect frozenEffect;
			frozenEffect.apply(registry);
			break;
		}
		case EffectType::Burning: {
			registry.emplace_or_replace<BurningTag>(entity);
			BurningEffect burningEffect;
			burningEffect.apply(registry);
			break;
		}
		case EffectType::Poisoned: {
			registry.emplace_or_replace<PoisonedTag>(entity);
			PoisonedEffect poisonedEffect;
			poisonedEffect.apply(registry);
			break;
		}
		case EffectType::Aggressive: {
			registry.emplace_or_replace<AggressiveTag>(entity);
			AggressiveEffect aggressiveEffect;
			aggressiveEffect.apply(registry);
			break;
		}
		default:
			break;
	}
}

void FrozenEffect::apply(entt::registry& registry)
{
	auto view = registry.view<FrozenTag, EnemyTag>();
	// speed = 0, disable shooting, etc.
	// call update	
}

void BurningEffect::apply(entt::registry& registry)
{
	auto view = registry.view<BurningTag, EnemyTag>();
	// call update
}

void PoisonedEffect::apply(entt::registry& registry)
{
	auto view = registry.view<PoisonedTag, EnemyTag>();
	// reduce speed, atk
	// call update
}	

void AggressiveEffect::apply(entt::registry& registry)
{
	auto check = registry.view<AggressiveTag, EnemyTag>();
	for (auto [entity, aggressiveTag] : check.each()) {
		auto stats = registry.try_get<SpeedComponent, AttackComponent>(entity);
		// Increase speed and attack power
	}
}