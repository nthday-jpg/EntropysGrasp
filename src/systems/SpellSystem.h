#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include "../resources/SpellLibrary.h"

class SpellSystem
{
private:
    std::unordered_map<std::string, float> castTimes; // Maps spell names to their cast times
    std::unordered_map<std::string, float> cooldowns; // Maps spell names to their cooldown times
    std::unordered_map<entt::entity, float> timeLeft;
public:
    void updateCastingSystem(entt::registry& registry, float dt, const SpellLibrary& spellLibrary);
    void updateCooldownSystem(entt::registry& registry, float dt);
    void updateDurationSystem(entt::registry& registry, float dt);
};