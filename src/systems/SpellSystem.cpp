#include "../components/statComponent.h"
#include "../behavior/EntityBehavior.h"
#include "../components/EntityTag.h"
#include "../components/Spell.h"
#include "BehaviorSystem.h"
#include "SpellSystem.h"

void SpellSystem::updateCastingSystem(entt::registry& registry, float dt, SpellLibrary spellLibrary) {
    auto view = registry.view<PlayerTag>();
    for (auto player : view)
        for (auto it = castTimes.begin(); it != castTimes.end();)
        {
            auto& mana = registry.get<ManaComponent>(player);
            const auto& spellData = spellLibrary.getSpell(it->first);
            if (cooldowns.find(it->first) == cooldowns.end() || cooldowns[it->first] == 0.0f)
            {
                if (mana.mana < spellData.manaCost)
                {
                    it = castTimes.erase(it); // Remove spell if not enough mana
                    continue;
                }
                mana.mana -= spellData.manaCost; // Deduct mana cost

                // Reduce the cast time
                it->second -= dt;
                if (it->second <= 0.0f)
                {
                    // Cast the spell
                    entt::entity spell = createSpell(registry, player, it->first, spellLibrary);
                    cooldowns[it->first] = spellData.cooldowns; // Set the cooldown for the spell
                    timeLeft[spell] = spellData.duration; // Set the duration for the spell
                    it = castTimes.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
}

void SpellSystem::updateCooldownSystem(entt::registry& registry, float dt) {
    for (auto it = cooldowns.begin(); it != cooldowns.end();)
    {
        it->second -= dt;
        if (it->second <= 0.0f)
        {
            it = cooldowns.erase(it);
        }
        else {
            ++it;
        }
    }
}

void SpellSystem::updateDurationSystem(entt::registry& registry, float dt) {
    for (auto it = timeLeft.begin(); it != timeLeft.end();)
    {
        it->second -= dt;
        if (it->second <= 0.0f)
        {
            it = timeLeft.erase(it);
            // remove spell effect
            registry.destroy(it->first);
        }
        else
        {
            ++it;
        }
    }
}
