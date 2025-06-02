#pragma once
#include <string>
#include <entt/entt.hpp>
#include "../components/lookingDirection.h"

enum class SpellEffect {
    Burn,
	Freeze,
    Heal,
    Shield,
    Teleport,
    SummonCreature,
	Poison
};

using BehaviorFunction = std::function<void(entt::entity, entt::registry&, float)>;

struct StraightMovement {};
struct HomingMovement { entt::entity target; };
struct OrbitMovement { entt::entity center; float radius; float speed; };

void straightMovementBehavior(entt::entity spell, entt::registry& registry, float dt) {
	auto& position = registry.get<Position>(spell);
	auto& velo = registry.get<Velocity>(spell);

	position += velo * dt;
}

void homingMovementBehavior(entt::entity spell, entt::entity enemy, entt::registry& registry, float dt) {
	// Implement homing movement behavior
}

void orbitMovementBehavior(entt::entity entity, entt::registry& registry, float dt) {
    // Implement orbit movement behavior
}

struct SpellData {
    std::string id;
	float damage;
    float manaCost;
    float castTime;
    float cooldowns;
    float speed;
    float size;
    float duration;
	SpellEffect effect;
	BehaviorFunction updateFunction;
};

std::unordered_map<std::string, SpellData> spellDatabase;

SpellData getSpellData(std::string spellName) {
    return spellDatabase[spellName];
}

struct OnActivateSpell {
    OnActivateSpell(std::string s) : spellid(s) {}
    std::string spellid;
};