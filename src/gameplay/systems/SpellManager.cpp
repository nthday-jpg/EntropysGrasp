#include "../components/StatComponent.h"
#include "../components/EntityTags.h"
#include "../components/Spell.h"
#include "../components/Hitbox.h"
#include "../components/MovementComponents.h"
#include "../../Utils/Random.h"
#include "BehaviorSystem.h"
#include "SpellManager.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "../components/CollisionEvent.h"

using namespace std;

const float PI = 3.14159265358979323846f;

vector<entt::entity> SpellManager::createSpell(entt::entity caster, SpellID spellID)
{
	const SpellLibrary& spellLibrary = SpellLibrary::getInstance(); // Assuming SpellLibrary is a singleton
    const SpellData& spellData = spellLibrary.getSpell(spellID);
    Position position = registry.get<Position>(caster);
    if (spellData.behaviorType == BehaviorType::Orbit)
    {
        position.x -= spellData.radius;
        position.y -= spellData.radius;
    }
    vector<entt::entity> spellEntities;
    vector<MovementDirection> directions = putDirection(spellID, spellData.count, caster);
    for (int i = 0; i < spellData.count; ++i)
    {
        entt::entity spellEntity = registry.create();

        registry.emplace<SpellTag>(spellEntity);
        registry.emplace<SpellID>(spellEntity, spellID); // Pass spellID directly without creating a temporary object
        registry.emplace<Position>(spellEntity, position.x + Random::getFloat(-50.0f, 50.0f), position.y + Random::getFloat(-50.0f, 50.0f));
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

vector<MovementDirection> SpellManager::putDirection(SpellID spellID, int count, entt::entity caster)
{
	const SpellLibrary& spellLibrary = SpellLibrary::getInstance(); // Assuming SpellLibrary is a singleton
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

void SpellManager::updateCastingSystem(float dt) 
{
    auto view = registry.view<PlayerTag>();
    for (auto player : view) 
    {
        for (auto it = castTimes.begin(); it != castTimes.end();)
        {
            auto& mana = registry.get<Mana>(player);
			const SpellLibrary& spellLibrary = SpellLibrary::getInstance(); // Assuming SpellLibrary is a singleton
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
                    vector<entt::entity> spell = createSpell(player, it->first);
                    for (size_t i = 0; i < spell.size(); ++i)
                    {
                        durations[spell[i]] = spellData.duration + Random::getFloat(-0.5f, 0.5f); // Set the duration for the spell
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

void SpellManager::updateCooldownSystem(float dt)
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

void SpellManager::updateDurationSystem(float dt)
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

void SpellManager::handleSpellCollision(const SpellReduction& event)
{
    entt::entity spellEntity = event.spell;
	durations[spellEntity] -= 10000.0f; // Reduce the duration of the spell by 0.5 seconds

}

void SpellManager::update(float dt) 
{
    updateCastingSystem(dt);
    updateCooldownSystem(dt);
    updateDurationSystem(dt);
}

void SpellManager::sinkEvents() 
{
    // Access dispatcher from registry context and connect to events
    if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>())
    {
        (*dispatcher)->sink<SpellReduction>().connect<&SpellManager::handleSpellCollision>(*this);
    }
    else
    {
        std::cerr << "Dispatcher not found in registry context!" << std::endl;
        assert(false); // Handle error appropriately
    }
}