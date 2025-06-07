#include "../components/movementComponents.h"
#include "../components/EntityTags.h"
#include "../components/statComponent.h"
#include "EntityBehavior.h"

entt::entity createSpell(entt::registry& registry, entt::entity caster, SpellID spellID, const SpellLibrary& spellLibrary) {
	SpellData spellData = spellLibrary.getSpell(spellID);
	Position position = registry.get<Position>(caster);

	auto spellEntity = registry.create();

	registry.emplace<SpellTag>(spellEntity);
	registry.emplace<SpellID>(spellEntity, SpellID{ spellID });	
	registry.emplace<Position>(spellEntity, position);
	registry.emplace<Attack>(spellEntity, spellData.damage);
	registry.emplace<Mana>(spellEntity, spellData.manaCost);
	registry.emplace<Speed>(spellEntity, spellData.speed);
	registry.emplace<Size>(spellEntity, spellData.size);
	registry.emplace<Radius>(spellEntity, spellData.radius);
	registry.emplace<BehaviorType>(spellEntity, spellData.behaviorType);
	registry.emplace<SpellEffect>(spellEntity, spellData.effect);

	return spellEntity;
}