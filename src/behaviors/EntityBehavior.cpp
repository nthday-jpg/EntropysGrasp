#include "../components/movementComponents.h"
#include "../components/EntityTags.h"
#include "../components/statComponent.h"
#include "EntityBehavior.h"

entt::entity createSpell(entt::registry& registry, entt::entity caster, SpellID spellID, const SpellLibrary& spellLibrary) {
	SpellData spellData = spellLibrary.getSpell(spellID);
	Position position = registry.get<Position>(caster);
	auto direction = registry.get<LookingDirection>(caster);

	entt::entity spellEntity = registry.create();

	registry.emplace<SpellTag>(spellEntity);
	registry.emplace<SpellID>(spellEntity, SpellID{ spellID });	
	registry.emplace<Position>(spellEntity, 100.0f, 100.0f);
	registry.emplace<Attack>(spellEntity, spellData.damage);
	registry.emplace<Mana>(spellEntity, spellData.manaCost);
	registry.emplace<Speed>(spellEntity, spellData.speed);
	registry.emplace<Velocity>(spellEntity, 0.0f, 0.0f);
	registry.emplace<Size>(spellEntity, spellData.size);
	registry.emplace<Radius>(spellEntity, spellData.radius);
	registry.emplace<BehaviorType>(spellEntity, BehaviorType::HomingPlayer);
	registry.emplace<SpellEffect>(spellEntity, spellData.effect);

	return spellEntity;
}