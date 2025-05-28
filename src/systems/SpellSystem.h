#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include "../components/Spell.h"

struct SpellCasting
{
	std::string spellName;
	float castTimeRemaining; // Time required to cast the spell
};

struct SpellCooldown
{
	std::unordered_map<std::string, float> cooldowns; // Maps spell names to their cooldown times
};


void updateCastingSystem(entt::registry& registry, float dt) {
    auto view = registry.view<SpellCasting>();
    for (auto entity : view) {
        auto& casting = view.get<SpellCasting>(entity);
        casting.castTimeRemaining -= dt;

        if (casting.castTimeRemaining <= 0.0f) {
            // Cast complete
            //triggerSpellEffect(registry, entity, casting.spellName);

            // Start cooldown
			auto& cooldowns = registry.get_or_emplace<SpellCooldown>(entity).cooldowns;
			auto spellData = getSpellData(casting.spellName);
			cooldowns[casting.spellName] = spellData.cooldowns;

            // Remove casting component
            registry.remove<SpellCasting>(entity);
        }
    }
}

void updateCooldownSystem(entt::registry& registry, float dt) {
    auto view = registry.view<SpellCooldown>();
    for (auto entity : view) {
        auto& cooldowns = view.get<SpellCooldown>(entity);

        for (auto it = cooldowns.cooldowns.begin(); it != cooldowns.cooldowns.end();) {
            it->second -= dt;
            if (it->second <= 0.0f) {
                it = cooldowns.cooldowns.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

bool tryCastSpell(entt::registry& registry, entt::entity entity, const std::string& spell_id) {
    const auto& cooldowns = registry.get_or_emplace<SpellCooldown>(entity);

    // Can't cast if already in cooldown or currently casting
    if (cooldowns.cooldowns.count(spell_id) > 0 || registry.all_of<SpellCasting>(entity)) {
        return false;
    }

    float cast_time = getSpellData(spell_id).castTime;

    if (cast_time > 0.0f) {
        registry.emplace_or_replace<SpellCasting>(entity, SpellCasting{ spell_id, cast_time });
    }
    else {
        //triggerSpellEffect(registry, entity, spell_id);
        registry.get_or_emplace<SpellCooldown>(entity).cooldowns[spell_id] = getSpellData(spell_id).cooldowns;
    }

    return true;
}
