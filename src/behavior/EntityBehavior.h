#pragma once
#include <entt/entt.hpp>
#include <cmath>
#include "../resources/SpellLibrary.h"
#include "../components/Behavior.h"

entt::entity createSpell(entt::registry& registry, entt::entity caster, std::string spellID, const SpellLibrary& spellLibrary);