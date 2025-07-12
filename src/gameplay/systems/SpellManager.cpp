#include "../components/StatComponent.h"
#include "../components/EntityTags.h"
#include "../components/Spell.h"
#include "../components/Hitbox.h"
#include "../components/MovementComponents.h"
#include "BehaviorSystem.h"
#include "SpellManager.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const float PI = 3.14159265358979323846f;

vector<entt::entity> SpellSystem::createSpell(entt::registry& registry, entt::entity caster, SpellID spellID, const SpellLibrary& spellLibrary)
{
    const SpellData& spellData = spellLibrary.getSpell(spellID);
    Position position = registry.get<Position>(caster);
    if (spellData.behaviorType == BehaviorType::Orbit)
    {
        position.x -= spellData.radius;
        position.y -= spellData.radius;
    }
    vector<entt::entity> spellEntities;
    vector<MovementDirection> directions = putDirection(registry, spellID, spellLibrary, spellData.count, caster);
    for (int i = 0; i < spellData.count; ++i)
    {
        entt::entity spellEntity = registry.create();

        registry.emplace<SpellTag>(spellEntity);
        registry.emplace<SpellID>(spellEntity, spellID); // Pass spellID directly without creating a temporary object
        registry.emplace<Position>(spellEntity, position);
        registry.emplace<Attack>(spellEntity, spellData.damage);
        registry.emplace<Mana>(spellEntity, spellData.manaCost);
        registry.emplace<Speed>(spellEntity, spellData.speed);
        registry.emplace<MovementDirection>(spellEntity, directions[i]);
        registry.emplace<Size>(spellEntity, spellData.size);
        registry.emplace<Radius>(spellEntity, spellData.radius);
        registry.emplace<BehaviorType>(spellEntity, spellData.behaviorType);
        registry.emplace<SpellEffect>(spellEntity, spellData.effect);
        registry.emplace<Hitbox>(spellEntity, Hitbox(15.0f, 15.0f, 0.0f, 0.0f));
        registry.emplace<RepelResistance>(spellEntity, 0.5f); // Example resistance value

        spellEntities.push_back(spellEntity);
    }
    return spellEntities;
}

vector<MovementDirection> SpellSystem::putDirection(entt::registry& registry, SpellID spellID, const SpellLibrary& spellLibrary, int count, entt::entity caster)
{
    const SpellData& spellData = spellLibrary.getSpell(spellID);
    LookingDirection direction = registry.get<LookingDirection>(caster);
    vector<MovementDirection> directions;
    if (spellData.behaviorType == BehaviorType::Orbit)
    {
        for (int i = 0; i < count; ++i)
        {
            float angleOffset = 2 * PI / count;
            float angle = i * angleOffset;
            float x = direction.x * std::cosf(angle) - direction.y * std::sinf(angle);
            float y = direction.x * std::sinf(angle) + direction.y * std::cosf(angle);
            directions.emplace_back(x, y);
        }
    }
    else
    {
        // Fire all spells in the caster's looking direction
        for (int i = 0; i < count; ++i)
        {
            directions.emplace_back(direction.x, direction.y);
        }
    }
    return directions;
}

void SpellSystem::updateCastingSystem(entt::registry& registry, float dt, const SpellLibrary& spellLibrary) 
{
    auto view = registry.view<PlayerTag>();
    for (auto player : view) 
    {
        for (auto it = castTimes.begin(); it != castTimes.end();)
        {
            auto& mana = registry.get<Mana>(player);
            const SpellData& spellData = spellLibrary.getSpell(it->first);
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
                    vector<entt::entity> spell = createSpell(registry, player, it->first, spellLibrary);
                    for (size_t i = 0; i < spell.size(); ++i)
                    {
                        durations[spell[i]] = spellData.duration; // Set the duration for the spell
                    }
                    cooldowns[it->first] = spellData.cooldowns; // Set the cooldown for the spell
                    it = castTimes.erase(it);
                }
                else 
                {
                    ++it;
                }
            }
        }
    }
}

void SpellSystem::updateCooldownSystem(entt::registry& registry, float dt) 
{
    for (auto it = cooldowns.begin(); it != cooldowns.end();)
    {
        it->second -= dt;
        if (it->second <= 0.0f)
        {
            it = cooldowns.erase(it);
        }
        else 
        {
            ++it;
        }
    }
}

void SpellSystem::updateDurationSystem(entt::registry& registry, float dt) 
{
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

void SpellSystem::update(entt::registry& registry, float dt, const SpellLibrary& spellLibrary) 
{
    updateCastingSystem(registry, dt, spellLibrary);
    updateCooldownSystem(registry, dt);
    updateDurationSystem(registry, dt);
}