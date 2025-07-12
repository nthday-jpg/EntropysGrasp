#include "EffectSystem.h"
#include "../components/StatComponent.h"
#include "../components/Entitytags.h"
#include "../components/EffectTags.h"
#include "../components/MovementComponents.h"

void applyEffect(entt::entity entity, entt::registry& registry, EffectType effectType, float dt) {
	switch (effectType) 
	{
		case EffectType::Frozen: 
		{
			registry.emplace_or_replace<FrozenTag>(entity);
			FrozenEffect frozenEffect;
			frozenEffect.apply(registry, dt);
			break;
		}
		case EffectType::Burning: 
		{
			registry.emplace_or_replace<BurningTag>(entity);
			BurningEffect burningEffect;
			burningEffect.apply(registry, dt);
			break;
		}
		case EffectType::Poisoned:
		{
			registry.emplace_or_replace<PoisonedTag>(entity);
			PoisonedEffect poisonedEffect;
			poisonedEffect.apply(registry, dt);
			break;
		}
		case EffectType::Aggressive: 
		{
			registry.emplace_or_replace<AggressiveTag>(entity);
			AggressiveEffect aggressiveEffect;
			aggressiveEffect.apply(registry, dt);
			break;
		}
		default:
			break;
	}
}

void FrozenEffect::apply(entt::registry& registry, float dt)
{
	auto view = registry.view<FrozenTag, EnemyTag>();
	for (auto [entity, frozenComponent] : view.each())
	{
		auto& tag = registry.get<FrozenTag>(entity);
		tag.remainingTime -= dt;
		if (!tag.applied)
		{
			// Store the original speed for restoration later
			tag.originalSpeed = registry.get<Speed>(entity).value;
			registry.replace<Speed>(entity, 0.0f);
			tag.applied = true;
		}
		if (tag.remainingTime <= 0.0f)
		{
			deactivate(registry, entity);
			registry.remove<FrozenTag>(entity);
		}
	}
}

void FrozenEffect::deactivate(entt::registry& registry, entt::entity entity)
{
	auto& tag = registry.get<FrozenTag>(entity);
	// Restore the original speed
	registry.replace<Speed>(entity, tag.originalSpeed);
}

void BurningEffect::apply(entt::registry& registry, float dt)
{
	auto view = registry.view<BurningTag, EnemyTag>();
	for (auto [entity, burningComponent] : view.each())
	{
		auto& tag = registry.get<BurningTag>(entity);
		tag.remainingTime -= dt;
		if (!tag.applied)
		{
			auto& health = registry.get<Health>(entity);
			health.current -= 5; // Apply damage only once
			tag.applied = true;
		}
		if (tag.remainingTime <= 0.0f)
		{
			deactivate(registry, entity);
			registry.remove<BurningTag>(entity);
		}
	}
}

void BurningEffect::deactivate(entt::registry& registry, entt::entity entity)
{
    // No need to restore anything since damage was one-time
    // But you could add a final effect here if needed
}

void PoisonedEffect::apply(entt::registry& registry, float dt)
{
    auto view = registry.view<PoisonedTag, EnemyTag>();
    for (auto [entity, poisonedComponent] : view.each()) 
	{
        auto& tag = registry.get<PoisonedTag>(entity);
        tag.remainingTime -= dt;
        if (!tag.applied) 
		{
            auto& speed = registry.get<Speed>(entity);
            auto& attack = registry.get<Attack>(entity);
            // Store original values
            tag.originalSpeed = speed.value;
            tag.originalAttack = attack.value;
            // Apply reductions
            speed.value -= 0.5f;
            attack.value -= 1.0f;
            tag.applied = true;
        }
        if (tag.remainingTime <= 0.0f)
		{
            deactivate(registry, entity);
            registry.remove<PoisonedTag>(entity);
        }
    }
}

void PoisonedEffect::deactivate(entt::registry& registry, entt::entity entity)
{
    auto& tag = registry.get<PoisonedTag>(entity);
    // Restore original values
    registry.replace<Speed>(entity, tag.originalSpeed);
    registry.replace<Attack>(entity, tag.originalAttack);
}

void AggressiveEffect::apply(entt::registry& registry, float dt)
{
	auto check = registry.view<AggressiveTag, EnemyTag>();
	for (auto [entity, aggressiveTag] : check.each()) 
	{
		auto& tag = registry.get<AggressiveTag>(entity);
		float threshold = registry.get<AggressiveTag>(entity).threshold;
		float currentHealth = registry.get<Health>(entity).current;
		if (currentHealth <= threshold) 
		{
			if (!tag.applied)
			{
				auto& speed = registry.get<Speed>(entity);
				auto& attack = registry.get<Attack>(entity);
				speed.value += 1.0f;
				attack.value += 2.0f;
				tag.applied = true;
			}
		}
		// Increase speed and attack power
		if (currentHealth <= 0) 
		{
			// If health is zero, deactivate the effect
			deactivate(registry, entity);
			registry.remove<AggressiveTag>(entity);
		}
	}
}

void AggressiveEffect::deactivate(entt::registry& registry, entt::entity entity)
{
	auto& tag = registry.get<PoisonedTag>(entity);
	// Restore original values
	registry.replace<Speed>(entity, tag.originalSpeed);
	registry.replace<Attack>(entity, tag.originalAttack);
}

void RepelEffect::apply(entt::registry& registry, float dt)
{
	auto view = registry.view<RepelTag>();
	for (auto [entity, repelTag] : view.each())
	{
		repelTag.remainingTime -= dt;
		if (repelTag.remainingTime <= 0.0f)
		{
			registry.remove<RepelTag>(entity);
		}
	}
}

void RepelEffect::deactivate(entt::registry& registry, entt::entity entity)
{
	// No specific deactivation logic needed for RepelEffect
	// The effect is applied continuously until the remaining time is zero
}