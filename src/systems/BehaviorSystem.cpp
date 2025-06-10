#include "BehaviorSystem.h"
#include"../resources/SpellLibrary.h"
#include "../components/Spell.h"
#include "../components/movementComponents.h"
#include "../components/EntityTags.h"
#include "../components/statComponent.h"
#include "../resources/EnemyLibrary.h"
#include "../components/hitbox.h"
#include "../Utils/VectorMath.h"
#include <SFML/System/Vector2.hpp>
#include <iostream>


void BehaviorSystem::initializeBehaviorMap() {
	behaviorMap[BehaviorType::Straight] = [](entt::entity entity, entt::entity caster, entt::registry& registry, float dt, const SpellLibrary& spellLibrary, const EnemyLibrary& enemyLibrary) {
		SpellID spellID = registry.get<SpellID>(entity);
		SpellData spellData = spellLibrary.getSpell(spellID);
		auto direction = registry.get<MovementDirection>(entity);
		if (!registry.all_of<Velocity>(entity)) 
		{	
			registry.emplace<Velocity>(entity, direction.x * spellData.speed, direction.y * spellData.speed);
		}
	};
	behaviorMap[BehaviorType::HomingEnemy] = [](entt::entity entity, entt::entity target, entt::registry& registry, float dt, const SpellLibrary& spellLibrary, const EnemyLibrary& enemyLibrary) {
		auto& position = registry.get<Position>(entity);
		Velocity velo;
		SpellID spellID = registry.get<SpellID>(entity);
		SpellData spellData = spellLibrary.getSpell(spellID);
		auto& enemyPosition = registry.get<Position>(target);

		// Calculate direction towards the target
		auto direction = enemyPosition - position;
		MovementDirection& movementDirection = registry.get<MovementDirection>(entity);
		movementDirection.x = direction.x;
		movementDirection.y = direction.y;
		normalize(movementDirection);

		if (magnitude(movementDirection) > 0) {
			velo.x = movementDirection.x * spellData.speed;
			velo.y = movementDirection.y * spellData.speed;
		}

		velo = registry.emplace_or_replace<Velocity>(entity, velo);
	};
	behaviorMap[BehaviorType::Orbit] = [](entt::entity entity, entt::entity center, entt::registry& registry, float dt, const SpellLibrary& spellLibrary, const EnemyLibrary& enemyLibrary) {
		Position position = registry.get<Position>(entity); //spell entity
		Position centerPosition = registry.get<Position>(center);
		auto size = registry.get<Hitbox>(center);
		centerPosition.x += size.width / 2 - 5; // Adjust center position to the center of the hitbox
		centerPosition.y += size.height / 2 - 6; // Adjust center position to the center of the hitbox

		Velocity velocity;
		Velocity velocityCenter = registry.get<Velocity>(center); // center entity
		
		SpellID spellID = registry.get<SpellID>(entity);
		SpellData spellData = spellLibrary.getSpell(spellID);
		float speed = spellData.speed;
		float radius = spellData.radius;

		sf::Vector2f distance = position - centerPosition;

		float devitate = magnitude(distance) - radius; 

		sf::Vector2f norm = normalize(distance);
		sf::Vector2f movementDir(-norm.y, norm.x); 

		velocity.x = movementDir.x * speed + velocityCenter.x - devitate * norm.x; 
		velocity.y = movementDir.y * speed + velocityCenter.y - devitate * norm.y;

		registry.emplace_or_replace<Velocity>(entity, velocity);
	};
	behaviorMap[BehaviorType::HomingPlayer] = [](entt::entity entity, entt::entity target, entt::registry& registry, float dt, const SpellLibrary& spellLibrary, const EnemyLibrary& enemyLibrary) {
		auto& position = registry.get<Position>(entity);
		Velocity velo;
		EnemyType enemyID = registry.get<EnemyType>(entity);
		EnemyData enemyData = enemyLibrary.getEnemyData(enemyID);
		auto& enemyPosition = registry.get<Position>(target);

		// Calculate direction towards the target
		auto direction = enemyPosition - position;
		MovementDirection& movementDirection = registry.get<MovementDirection>(entity);
		movementDirection.x = direction.x;
		movementDirection.y = direction.y;
		normalize(movementDirection);

		if (magnitude(movementDirection) > 0) {
			velo.x = movementDirection.x * enemyData.speed.value;
			velo.y = movementDirection.y * enemyData.speed.value;
		}

		velo = registry.emplace_or_replace<Velocity>(entity, velo);
	};
}

void BehaviorSystem::updateBehavior(entt::registry& registry, float dt, const SpellLibrary& spellLibrary, const EnemyLibrary& enemyLibrary) {
	auto view = registry.view<BehaviorType>();
	for (auto [entity, behaviorType] : view.each()) {
		if (registry.all_of<SpellTag>(entity)) {
			SpellID spellName = registry.get<SpellID>(entity);
			SpellData spell = spellLibrary.getSpell(spellName);
			auto it = behaviorMap.find(spell.behaviorType);
			if (it != behaviorMap.end()) {
				if (spell.behaviorType == BehaviorType::Straight) {
					auto view = registry.view<PlayerTag>();
					for (auto player : view) {
						it->second(entity, player, registry, dt, spellLibrary, enemyLibrary);
					}
				}
				else if (spell.behaviorType == BehaviorType::HomingEnemy) {
					auto view = registry.view<PlayerTag>();
					for (auto player : view) {
						it->second(entity, player, registry, dt, spellLibrary, enemyLibrary);
					}
				}
				else if (spell.behaviorType == BehaviorType::Orbit) {
					auto view = registry.view<PlayerTag>();
					for (auto& player : view) {
						it->second(entity, player, registry, dt, spellLibrary, enemyLibrary);
					}
				}
			}
		}
		else if (registry.all_of<EnemyTag>(entity)) {
			EnemyType enemyID = registry.get<EnemyType>(entity);
			EnemyData enemy = enemyLibrary.getEnemyData(enemyID);
			auto view = registry.view<PlayerTag>();
			for (auto player : view) {
				auto it = behaviorMap.find(enemy.behaviorType);
				if (it != behaviorMap.end()) {
					it->second(entity, player, registry, dt, spellLibrary, enemyLibrary);
				}
			}
		}
	}
}