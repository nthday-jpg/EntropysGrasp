#include "../components/movementComponents.h"
#include "../components/EntityTag.h"
#include "EntityBehavior.h"

entt::entity createSpell(entt::registry& registry, entt::entity caster, std::string spellID, const SpellLibrary& spellLibrary) {
	SpellData spellData = spellLibrary.getSpell(spellID);
	Position position = registry.get<Position>(caster);

	auto spellEntity = registry.create();

	registry.emplace<SpellTag>(spellEntity);
	registry.emplace<SpellID>(spellEntity, SpellID{ spellID });	
	registry.emplace<Position>(spellEntity, position);
	registry.emplace<Velocity>(spellEntity, Velocity{ spellData.speed, 0.0f });
	registry.emplace<BehaviorType>(spellEntity, spellData.behaviorType);
	registry.emplace<SpellEffect>(spellEntity, spellData.effect);

	return spellEntity;
}