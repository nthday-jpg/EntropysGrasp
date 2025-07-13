#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include "../../manager/SpellLibrary.h"

class SpellSystem
{
	entt::registry& registry; // Reference to the entity registry
public:
    std::unordered_map<SpellID, float> castTimes; // Maps spell names to their cast times
    std::unordered_map<SpellID, float> cooldowns; // Maps spell names to their cooldown times
    std::unordered_map<entt::entity, float> durations;

	SpellSystem(entt::registry& registry) : registry(registry) {}

    std::vector<entt::entity> createSpell(entt::entity caster, SpellID spellID);
    std::vector<MovementDirection> putDirection(SpellID spellID, int count, entt::entity caster);
    void updateCastingSystem(float dt);
    void updateCooldownSystem(float dt);
    void updateDurationSystem(float dt);
    void update(float dt);
};