#pragma once
#include <entt/entt.hpp>
#include"../resources/SpellLibrary.h"
#include <unordered_map>
#include "../components/Spell.h"
#include "../behavior/EntityBehavior.h"

class BehaviorSystem {
	std::unordered_map<SpellBehaviorType, BehaviorFunction> behaviorMap;
public:
	void updateBehavior(entt::registry& registry, float dt, SpellLibrary spellLibrary) {
		auto view = registry.view<OnActivateSpell>();
		for (auto [entity, activeSpell] : view.each()) {
			std::string spellName = activeSpell.spellID;
			SpellData spell = spellLibrary.getSpell(spellName);
			auto it = behaviorMap.find(spell.behaviorType);
			if (it != behaviorMap.end()) {
				//it->second(entity, registry, dt, spellLibrary);
			}
		}
	}
};

