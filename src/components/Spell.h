#pragma once
#include <string>
#include <entt/entt.hpp>
#include "../components/lookingDirection.h"
#include "Behavior.h"

enum class SpellEffect {
    Burn,
	Freeze,
    Heal,
    Shield,
    Teleport,
    SummonCreature,
	Poison,
	Penetrate,
	Knockback,
	Explosion,
};

struct SpellData {
    float damage;
    float manaCost;
    float castTime;
    float cooldowns;
    float speed;
    float size;
    float duration;
    float radius;
    SpellEffect effect;
    BehaviorType behaviorType;
};

enum class SpellID {
	Fireball,
	IceSpike,
	Heal,
	Shield,
	Teleport,
	SummonCreature,
	PoisonCloud,
	PenetratingShot,
	KnockbackBlast,
	ExplosionWave,
};