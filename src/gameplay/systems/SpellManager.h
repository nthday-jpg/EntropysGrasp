#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include "../../manager/SpellLibrary.h"

class SpellSystem
{
public:
    std::unordered_map<SpellID, float> castTimes; // Maps spell names to their cast times
    std::unordered_map<SpellID, float> cooldowns; // Maps spell names to their cooldown times
    std::unordered_map<entt::entity, float> durations;

    std::vector<entt::entity> createSpell(entt::registry& registry, entt::entity caster, SpellID spellID, const SpellLibrary& spellLibrary);
    std::vector<MovementDirection> putDirection(entt::registry& registry, SpellID spellID, const SpellLibrary& spellLibrary, int count, entt::entity caster);
    void updateCastingSystem(entt::registry& registry, float dt, const SpellLibrary& spellLibrary);
    void updateCooldownSystem(entt::registry& registry, float dt);
    void updateDurationSystem(entt::registry& registry, float dt);
    void update(entt::registry& registry, float dt, const SpellLibrary& spellLibrary);
};