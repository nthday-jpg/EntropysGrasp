#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include "../components/Spell.h"
#include "../components/statComponent.h"



struct StraightMovement{};
struct HomingMovement {};
struct OrbitMovement {};

struct SpellCasting
{
	std::unordered_map<std::string, float> castTimes; // Maps spell names to their cast times
};

struct SpellCooldown
{
	std::unordered_map<std::string, float> cooldowns; // Maps spell names to their cooldown times
};

struct SpellDuration
{
    std::unordered_map<std::string, float> timeLeft;
};

void updateCastingSystem(entt::registry& registry, float dt) {
    auto view = registry.view<SpellCasting>();
    for (auto entity : view) {
        auto& casting = view.get<SpellCasting>(entity);
        for (auto it = casting.castTimes.begin(); it != casting.castTimes.end();)
        {
            auto& mana = registry.get<ManaComponent>(entity);
            const auto& spellData = getSpellData(it->first);
            if (mana.mana < spellData.manaCost)
            {
                it = casting.castTimes.erase(it); // Remove spell if not enough mana
                continue;
            }
            mana.mana -= spellData.manaCost; // Deduct mana cost

            // Reduce the cast time
            it->second -= dt;
            if (it->second <= 0.0f)
            {
                // Cast the spell
                //triggerSpellEffect(registry, entity, it->first);
                auto& cooldowns = registry.get_or_emplace<SpellCooldown>(entity).cooldowns;
                cooldowns[it->first] = spellData.cooldowns; // Set the cooldown for the spell
                registry.get_or_emplace<SpellDuration>(entity).timeLeft[it->first] = spellData.duration; // Set the duration for the spell
                it = casting.castTimes.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void updateCooldownSystem(entt::registry& registry, float dt) {
    auto view = registry.view<SpellCooldown>();
    for (auto entity : view) 
    {
        auto& cooldowns = view.get<SpellCooldown>(entity);

        for (auto it = cooldowns.cooldowns.begin(); it != cooldowns.cooldowns.end();) 
        {
            it->second -= dt;
            if (it->second <= 0.0f) 
            {
                it = cooldowns.cooldowns.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void updateDurationSystem(entt::registry& registry, float dt) {
	const auto& view = registry.view<SpellDuration>();
    for (auto entity : view) 
    {
        auto& duration = view.get<SpellDuration>(entity);
        for (auto it = duration.timeLeft.begin(); it != duration.timeLeft.end();) 
        {
            it->second -= dt;
            if (it->second <= 0.0f)
            {
                // Remove the spell effect
				// call removeSpellEffect(registry, entity, it->first);
                it = duration.timeLeft.erase(it);
            }
            else 
            {
                ++it;
            }
        }
	}
}

void triggerSpellEffect(entt::registry& registry, entt::entity caster, const std::string& spell_id) {
    const auto& data = getSpellData(spell_id);

    auto spellEntity = registry.create();
    registry.emplace<whatSpell>(spellEntity, whatSpell{ spell_id });

    //if (data.effectId == "straight") {
    //    registry.emplace<StraightMovement>(spellEntity);
    //}
    //else if (data.effectId == "homing") {
    //    registry.emplace<HomingMovement>(spellEntity);
    //}

    // Add more components (position, velocity, damage, duration,...)
}
