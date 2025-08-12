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
#include <string>
#include "../components/CollisionEvent.h"
#include "../../manager/TextureManager.h"
#include "../components/Animation.h"

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
        registry.emplace<Position>(spellEntity, position);
        registry.emplace<Speed>(spellEntity, spellData.speed);
        registry.emplace<MovementDirection>(spellEntity, directions[i]);
        registry.emplace<BehaviorType>(spellEntity, spellData.behaviorType);
        registry.emplace<Hitbox>(spellEntity, Hitbox(15.0f, 15.0f, 0.0f, 0.0f));
        registry.emplace<RepelResistance>(spellEntity, 0.5f); // Example resistance value

		std::string name = "pulse";
        AnimationComponent animComp;
		int ID = (int)spellID;
		animComp.name = name;
		animComp.currentState = AnimationState::Attacking;
		animComp.currentDirection = Direction::Down;
		animComp.currentFrame = { 0, 0 };
		animComp.timer = 0.0f;
		registry.emplace<AnimationComponent>(spellEntity, animComp);

        sf::Texture* texture = TextureManager::getInstance().getTexture(name);
        sf::IntRect textureRect({0, 0}, {32, 32});
		sf::Sprite spellSprite(*texture);
		spellSprite.setTextureRect(textureRect);
		spellSprite.setOrigin({textureRect.size.x/2.0f, textureRect.size.y/2.0f}); // Set origin to center
		registry.emplace<sf::Sprite>(spellEntity, spellSprite);

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
            else 
            {
                ++it; // Skip to the next spell if it's on cooldown
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
	SpellID spellID = registry.get<SpellID>(spellEntity);
	durations[spellEntity] -= SpellLibrary::getInstance().getSpell(spellID).timeDecreasePerHit; // Reduce the duration of the spell by 0.5 seconds
}

void SpellManager::castSpell(SpellID spellID) 
{
    castTimes[spellID] = SpellLibrary::getInstance().getSpell(spellID).castTime;
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

SpellID SpellManager::currentSpell() const
{
	return currentSpellID;
}

void SpellManager::setCurrentSpell(SpellID spellID)
{
    currentSpellID = spellID;
}

SpellID SpellManager::getSpellID(int i) const
{
    if (i < 0 || i >= usableSpells.size())
    {
        std::cerr << "Index out of bounds: " << i << std::endl;
        assert(false); // Handle error appropriately
        return SpellID::Unknown; // Return a default value or handle the error as needed
	}
	return usableSpells[i - 1];
}