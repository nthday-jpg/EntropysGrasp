#pragma once
#include <entt/entt.hpp>
#include <cmath>
#include "../resources/SpellLibrary.h"
#include "../components/Behavior.h"
#include "../components/movementComponents.h"
#include <vector>

std::vector<entt::entity> createSpell(entt::registry& registry, entt::entity caster, SpellID spellID, const SpellLibrary& spellLibrary, int count);
std::vector<MovementDirection> check(SpellID spellID, const SpellLibrary& spellLibrary, int count);