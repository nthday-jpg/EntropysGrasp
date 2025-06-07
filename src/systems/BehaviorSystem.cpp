#include "BehaviorSystem.h"
#include"../resources/SpellLibrary.h"
#include "../components/Spell.h"
#include "../components/movementComponents.h"
#include "../components/EntityTag.h"

void BehaviorSystem::initializeBehaviorMap() {
	behaviorMap[BehaviorType::Straight] = [](entt::entity entity, entt::entity /*unused*/, entt::registry& registry, float dt, const SpellLibrary& spellLibrary) {
		auto& position = registry.get<Position>(entity);
		auto& velo = registry.get<Velocity>(entity);
		std::string spellID = registry.get<SpellID>(entity).spellID;
		SpellData spellData = spellLibrary.getSpell(spellID);
		auto& direction = registry.get<MovementDirection>(entity);

		velo.x = direction.x * spellData.speed;
		velo.y = direction.y * spellData.speed;
		position += velo * dt;
		};
	behaviorMap[BehaviorType::Homing] = [](entt::entity entity, entt::entity target, entt::registry& registry, float dt, const SpellLibrary& spellLibrary) {
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
	behaviorMap[BehaviorType::Orbit] = [](entt::entity entity, entt::entity center, entt::registry& registry, float dt, const SpellLibrary& spellLibrary) {
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

void BehaviorSystem::updateBehavior(entt::registry& registry, float dt, const SpellLibrary& spellLibrary) {
	auto view = registry.view<BehaviorType>();
	for (auto [entity, behaviorType] : view.each()) {
		if (registry.all_of<SpellTag>(entity)) {
			auto& spellName = registry.get<SpellID>(entity).spellID;
			SpellData spell = spellLibrary.getSpell(spellName);
			auto it = behaviorMap.find(spell.behaviorType);
			if (it != behaviorMap.end()) {
				if (spell.behaviorType == BehaviorType::Straight) {
					it->second(entity, entity, registry, dt, spellLibrary);
				}
				else if (spell.behaviorType == BehaviorType::Homing) {
					//it->second(entity, registry, dt, spellLibrary);
				}
				else if (spell.behaviorType == BehaviorType::Orbit) {
					auto view = registry.view<PlayerTag>();
					for (auto player : view) {
						it->second(entity, player, registry, dt, spellLibrary);
					}
				}
			}
		}
		else if (registry.all_of<EnemyTag>(entity)) {
			//auto& enemy = registry.get<EnemyLibrary>(entity);
			//auto it = behaviorMap.find(behaviorType);
			//if (it != behaviorMap.end()) {
			//	it->second(entity, registry, dt, EnemyLibrary);
			//}
		}
	}
}