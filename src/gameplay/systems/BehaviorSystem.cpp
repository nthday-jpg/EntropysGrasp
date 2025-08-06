#include "BehaviorSystem.h"
#include "../../manager/SpellLibrary.h"
#include "../components/Spell.h"
#include "../components/MovementComponents.h"
#include "../components/EntityTags.h"
#include "../components/StatComponent.h"
#include "../../manager/EnemyLibrary.h"
#include "../components/Hitbox.h"
#include "../../utils/VectorMath.h"
#include <SFML/System/Vector2.hpp>
#include <iostream>

BehaviorSystem::BehaviorSystem()
{
	initializeBehaviorMap();
}

void BehaviorSystem::initializeBehaviorMap()
{
	behaviorMap[BehaviorType::Straight] = [](entt::entity entity, entt::entity caster, entt::registry& registry, float dt)
	{
		SpellLibrary& spellLibrary = SpellLibrary::getInstance();
		const SpellID& spellID = registry.get<SpellID>(entity);
		const SpellData& spellData = spellLibrary.getSpell(spellID);
		const MovementDirection& direction = registry.get<MovementDirection>(entity);
		if (!registry.all_of<Velocity>(entity)) 
		{	
			registry.emplace<Velocity>(entity, direction.x * spellData.speed, direction.y * spellData.speed);
		}
	};
	behaviorMap[BehaviorType::HomingEnemy] = [](entt::entity entity, entt::entity target, entt::registry& registry, float dt)
	{
		SpellLibrary& spellLibrary = SpellLibrary::getInstance();
		Position position = registry.get<Position>(entity);
		Velocity velo;
		SpellID spellID = registry.get<SpellID>(entity);
		const SpellData& spellData = spellLibrary.getSpell(spellID);
		Position enemyPosition = registry.get<Position>(target);

		// Calculate direction towards the target
		sf::Vector2f direction = enemyPosition - position;
		MovementDirection& movementDirection = registry.get<MovementDirection>(entity);
		movementDirection.x = static_cast<float>(direction.x);
		movementDirection.y = static_cast<float>(direction.y);
		normalize(movementDirection);

		if (magnitude(movementDirection) > 0) 
		{
			velo.x = movementDirection.x * spellData.speed;
			velo.y = movementDirection.y * spellData.speed;
		}

		velo = registry.emplace_or_replace<Velocity>(entity, velo);
	};
	behaviorMap[BehaviorType::Orbit] = [](entt::entity entity, entt::entity center, entt::registry& registry, float dt)
	{
		SpellLibrary& spellLibrary = SpellLibrary::getInstance();
		Position position = registry.get<Position>(entity); //spell entity
		Position centerPosition = registry.get<Position>(center);
		Hitbox size = registry.get<Hitbox>(center);
		centerPosition.x += size.width / 2 - 5; // Adjust center position to the center of the hitbox
		centerPosition.y += size.height / 2 - 6; // Adjust center position to the center of the hitbox

		Velocity velocity;
		Velocity velocityCenter = registry.get<Velocity>(center); // center entity
		
		SpellID spellID = registry.get<SpellID>(entity);
		const SpellData& spellData = spellLibrary.getSpell(spellID);
		float speed = spellData.speed;
		float radius = spellData.radius;

		sf::Vector2f distance = position - centerPosition;

		float devitate = magnitude(distance) - radius; 

		normalize(distance);
		sf::Vector2f movementDir(-distance.y, distance.x);

		velocity.x = movementDir.x * speed + velocityCenter.x - devitate * distance.x;
		velocity.y = movementDir.y * speed + velocityCenter.y - devitate * distance.y;

		registry.emplace_or_replace<Velocity>(entity, velocity);
	};
	behaviorMap[BehaviorType::HomingPlayer] = [](entt::entity entity, entt::entity target, entt::registry& registry, float dt)
	{
		EnemyLibrary& enemyLibrary = EnemyLibrary::getInstance();
		Position position = registry.get<Position>(entity);
		Velocity velo;
		EnemyType enemyID = registry.get<EnemyType>(entity);
		const EnemyData& enemyData = enemyLibrary.getEnemyData(enemyID);
		Position enemyPosition = registry.get<Position>(target);

		// Calculate direction towards the target
		sf::Vector2f direction = enemyPosition - position;
		MovementDirection& movementDirection = registry.get<MovementDirection>(entity);
		movementDirection.x = static_cast<float>(direction.x);
		movementDirection.y = static_cast<float>(direction.y);
		normalize(movementDirection);

		if (magnitude(movementDirection) > 0) 
		{
			velo.x = movementDirection.x * enemyData.speed.value;
			velo.y = movementDirection.y * enemyData.speed.value;
		}

		velo = registry.emplace_or_replace<Velocity>(entity, velo);
	};
}

void BehaviorSystem::updateBehavior(entt::registry& registry, float dt)
{
	SpellLibrary& spellLibrary = SpellLibrary::getInstance();
	EnemyLibrary& enemyLibrary = EnemyLibrary::getInstance();
	auto view = registry.view<BehaviorType>();
	for (auto [entity, behaviorType] : view.each()) 
	{
		if (registry.all_of<SpellTag>(entity)) 
		{
			SpellID spellName = registry.get<SpellID>(entity);
			const SpellData& spell = spellLibrary.getSpell(spellName);
			auto it = behaviorMap.find(spell.behaviorType);
			if (it != behaviorMap.end()) 
			{
				if (spell.behaviorType == BehaviorType::Straight) 
				{
					auto view = registry.view<PlayerTag>();
					for (entt::entity player : view) 
					{
						it->second(entity, player, registry, dt);
					}
				}
				else if (spell.behaviorType == BehaviorType::HomingEnemy) 
				{
					auto view = registry.view<PlayerTag>();
					for (entt::entity player : view)
					{
						it->second(entity, player, registry, dt);
					}
				}
				else if (spell.behaviorType == BehaviorType::Orbit) 
				{
					auto view = registry.view<PlayerTag>();
					for (entt::entity player : view)
					{
						it->second(entity, player, registry, dt);
					}
				}
			}
		}
		else if (registry.all_of<EnemyTag>(entity)) 
		{
			EnemyType enemyID = registry.get<EnemyType>(entity);
			const EnemyData& enemy = enemyLibrary.getEnemyData(enemyID);
			auto view = registry.view<PlayerTag>();
			for (entt::entity player : view)
			{
				auto it = behaviorMap.find(enemy.behaviorType);
				if (it != behaviorMap.end()) 
				{
					it->second(entity, player, registry, dt);
				}
			}
		}
	}
}