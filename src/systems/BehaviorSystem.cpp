#include "BehaviorSystem.h"
#include"../resources/SpellLibrary.h"
#include "../components/Spell.h"
#include "../components/movementComponents.h"
#include "../components/EntityTags.h"
#include "../components/statComponent.h"
#include "../resources/EnemyLibrary.h"

void BehaviorSystem::initializeBehaviorMap() {
	behaviorMap[BehaviorType::Straight] = [](entt::entity entity, entt::entity /*unused*/, entt::registry& registry, float dt, const SpellLibrary& spellLibrary/*, const EnemyLibrary& enemyLibrary*/) {
		//SpellID spellID = registry.get<SpellID>(entity);
		//SpellData spellData = spellLibrary.getSpell(spellID);
		//auto direction = registry.get<LookingDirection>(entity);
		//registry.emplace_or_replace<Velocity>(entity, direction.x * spellData.speed, direction.y * spellData.speed);
	};
	behaviorMap[BehaviorType::HomingEnemy] = [](entt::entity entity, entt::entity target, entt::registry& registry, float dt, const SpellLibrary& spellLibrary/*, const EnemyLibrary& enemyLibrary*/) {
		auto& position = registry.get<Position>(entity);
		Velocity velo;
		SpellID spellID = registry.get<SpellID>(entity);
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

		velo = registry.emplace_or_replace<Velocity>(entity, velo);
	};
	behaviorMap[BehaviorType::Orbit] = [](entt::entity entity, entt::entity center, entt::registry& registry, float dt, const SpellLibrary& spellLibrary/*, const EnemyLibrary& enemyLibrary*/) {
		auto& position = registry.get<Position>(entity);
		SpellID spellID = registry.get<SpellID>(entity);
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
	//behaviorMap[BehaviorType::HomingPlayer] = [](entt::entity entity, entt::entity target, entt::registry& registry, float dt, const SpellLibrary& spellLibrary, const EnemyLibrary& enemyLibrary) {
	//	auto& position = registry.get<Position>(entity);
	//	Velocity velo;
	//	EnemyType enemyID = registry.get<EnemyType>(entity);
	//	EnemyData enemyData = enemyLibrary.getEnemyData(enemyID);
	//	auto& enemyPosition = registry.get<Position>(target);

	//	// Calculate direction towards the target
	//	auto direction = enemyPosition - position;
	//	float distance = sqrt(pow(direction.x, 2) + pow(direction.y, 2));

	//	if (distance > 0) {
	//		direction /= distance; // Normalize the direction vector
	//		velo.x = direction.x * enemyData.speed.value;
	//		velo.y = direction.y * enemyData.speed.value;
	//	}

	//	velo = registry.emplace_or_replace<Velocity>(entity, velo);
	//};
}

void BehaviorSystem::updateBehavior(entt::registry& registry, float dt, const SpellLibrary& spellLibrary/*, const EnemyLibrary& enemyLibrary*/) {
	auto view = registry.view<BehaviorType>();
	for (auto [entity, behaviorType] : view.each()) {
		if (registry.all_of<SpellTag>(entity)) {
			SpellID spellName = registry.get<SpellID>(entity);
			SpellData spell = spellLibrary.getSpell(spellName);
			auto it = behaviorMap.find(spell.behaviorType);
			if (it != behaviorMap.end()) {
				if (spell.behaviorType == BehaviorType::Straight) {
					it->second(entity, entity, registry, dt, spellLibrary/*, enemyLibrary*/);
				}
				else if (spell.behaviorType == BehaviorType::HomingEnemy) {
					//it->second(entity, registry, dt, spellLibrary);
				}
				else if (spell.behaviorType == BehaviorType::Orbit) {
					auto view = registry.view<PlayerTag>();
					for (auto player : view) {
						it->second(entity, player, registry, dt, spellLibrary/*, enemyLibrary*/);
					}
				}
			}
		}
		else if (registry.all_of<EnemyTag>(entity)) {
			//EnemyType enemyID = registry.get<EnemyType>(entity);
			//EnemyData enemy = enemyLibrary.getEnemyData(enemyID);
			//auto view = registry.view<PlayerTag>();
			//for (auto player : view) {
			//	auto it = behaviorMap.find(enemy.behaviorType);
			//	if (it != behaviorMap.end()) {
			//		it->second(entity, player, registry, dt, spellLibrary, enemyLibrary);
			//	}
			//}
		}
	}
}