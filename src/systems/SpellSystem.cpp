#include "../components/statComponent.h"
#include "../behaviors/EntityBehavior.h"
#include "../components/EntityTags.h"
#include "../components/Spell.h"
#include "BehaviorSystem.h"
#include "SpellSystem.h"
#include <iostream>
#include <vector>

using namespace std;

void SpellSystem::updateCastingSystem(entt::registry& registry, float dt, const SpellLibrary& spellLibrary) {
    auto view = registry.view<PlayerTag>();
    for (auto player : view)
        for (auto it = castTimes.begin(); it != castTimes.end();)
        {
            auto& mana = registry.get<Mana>(player);
            SpellData spellData = spellLibrary.getSpell(it->first);
            if (cooldowns.find(it->first) == cooldowns.end() || cooldowns[it->first] == 0.0f)
            {
                if (mana.value < spellData.manaCost)
                {
                    it = castTimes.erase(it); // Remove spell if not enough mana
                    continue;
                }
                mana.value -= spellData.manaCost; // Deduct mana cost

                // Reduce the cast time
                it->second -= dt;
                if (it->second <= 0.0f)
                {
                    // Cast the spell
					int count = 1; // Default to 1, can be modified based on spellData
					cout << "ok1" << endl;
                    vector<entt::entity> spell = createSpell(registry, player, it->first, spellLibrary, count);
					cout << "ok2" << endl;
                    for (int i = 0; i < spell.size(); ++i)
                    {
						cout << "ok3" << endl;
                        durations[spell[i]] = spellData.duration; // Set the duration for the spell
					}
					cout << "ok4" << endl;
                    cooldowns[it->first] = spellData.cooldowns; // Set the cooldown for the spell
					cout << "ok5" << endl;
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
    for (auto it = durations.begin(); it != durations.end();)
    {
        it->second -= dt;
        if (it->second <= 0.0f)
        {
            // remove spell effect
            registry.destroy(it->first);
            it = durations.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void SpellSystem::update(entt::registry& registry, float dt, const SpellLibrary& spellLibrary) {
    updateCastingSystem(registry, dt, spellLibrary);
    updateCooldownSystem(registry, dt);
    updateDurationSystem(registry, dt);
}