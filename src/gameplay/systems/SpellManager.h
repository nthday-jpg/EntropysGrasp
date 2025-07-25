#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include "../../manager/SpellLibrary.h"
#include "../components/MovementComponents.h"
#include "../components/CollisionEvent.h"

class SpellManager
{
	entt::registry& registry; // Reference to the entity registry
public:
    std::unordered_map<SpellID, float> castTimes; // Maps spell names to their cast times
    std::unordered_map<SpellID, float> cooldowns; // Maps spell names to their cooldown times
    std::unordered_map<entt::entity, float> durations;

	SpellManager(entt::registry& registry) : registry(registry) {}

    std::vector<entt::entity> createSpell(entt::entity caster, SpellID spellID);
    std::vector<MovementDirection> putDirection(SpellID spellID, int count, entt::entity caster);
    void updateCastingSystem(float dt);
    void updateCooldownSystem(float dt);
    void updateDurationSystem(float dt);
	void handleSpellCollision(const SpellReduction& event);
    void update(float dt);
	void sinkEvents();
};