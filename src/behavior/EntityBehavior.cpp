#include "../components/movementComponents.h"
#include "../components/Spell.h"
#include "EntityBehavior.h"

void initializeBehaviorMap() {
	behaviorMap[SpellBehaviorType::Straight] = [](entt::entity entity, entt::entity /*unused*/, entt::registry& registry, float dt, SpellLibrary spellLibrary) {
		auto& position = registry.get<Position>(entity);
		auto& velo = registry.get<Velocity>(entity);
		std::string spellID = registry.get<SpellID>(entity).spellID;
		SpellData spellData = spellLibrary.getSpell(spellID);
		auto& direction = registry.get<MovementDirection>(entity);

		velo.x = direction.x * spellData.speed;
		velo.y = direction.y * spellData.speed;
		position += velo * dt;
	};
	behaviorMap[SpellBehaviorType::Homing] = [](entt::entity entity, entt::entity target, entt::registry& registry, float dt, SpellLibrary spellLibrary) {
		auto& position = registry.get<Position>(entity);
		auto& velo = registry.get<Velocity>(entity);
		std::string spellID = registry.get<SpellID>(entity).spellID;
		SpellData spellData = spellLibrary.getSpell(spellID);
		auto& enemyPosition = registry.get<Position>(target);

		// Calculate direction towards the target
		auto direction = enemyPosition - position;
		float distance = sqrt(pow(direction.x, 2) + pow(direction.y, 2));

		if (distance > 0) {
			direction /= distance; // Normalize the direction vector
			velo.x = direction.x * spellData.speed;
			velo.y = direction.y * spellData.speed;
		}
		position += velo * dt; // Move the spell towards the enemy
	};
	behaviorMap[SpellBehaviorType::Orbit] = [](entt::entity entity, entt::entity center, entt::registry& registry, float dt, SpellLibrary spellLibrary) {
		auto& position = registry.get<Position>(entity);
		std::string spellID = registry.get<SpellID>(entity).spellID;
		SpellData spellData = spellLibrary.getSpell(spellID);
		auto& centerPosition = registry.get<Position>(center);
		float speed = spellData.speed;
		float radius = spellData.radius;
		// Calculate the angle of the orbit
		float angle = dt * speed;

		// Update position based on orbit radius and angle
		position.x = centerPosition.x + radius * cos(angle);
		position.y = centerPosition.y + radius * sin(angle);
	};
}

entt::entity triggerSpellEffect(entt::registry& registry, entt::entity caster, std::string spellID, SpellLibrary spellLibrary) {
	SpellData spellData = spellLibrary.getSpell(spellID);
	Position position = registry.get<Position>(caster);

	auto spellEntity = registry.create();

	registry.emplace<OnActivateSpell>(spellEntity, OnActivateSpell{ spellID });
	registry.emplace<Position>(spellEntity, position);
	registry.emplace<Velocity>(spellEntity, Velocity{ spellData.speed, 0.0f });
	registry.emplace<SpellBehaviorType>(spellEntity, spellData.behaviorType);
	registry.emplace<SpellEffect>(spellEntity, spellData.effect);

	return spellEntity;
}