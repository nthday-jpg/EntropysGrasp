#pragma once
#include <entt/entt.hpp>
#include <vector>
#include "../systems/CollisionSystem.h"
#include <iostream>

// This file defines the CombatSystem class which handles combat-related logic
// including collision handling, damage application, and spell effects.


class CombatSystem {
	entt::registry& registry;
	entt::dispatcher& dispatcher;
	 
public:
	void handleCollision(std::vector<CollisionEvent>& collisionEvents);
	void resolveEnemySpellCollision();
	void applyDamage(float dmg, entt::entity);

	CombatSystem(entt::registry& reg, entt::dispatcher& disp)
		: registry(reg), dispatcher(disp) {
	}
};

void CombatSystem::handleCollision(std::vector<CollisionEvent>& collisionEvents) {
	for (auto& event : collisionEvents)
	{
		using CT = CollisionType;
		/*
		* Player = 0
		* Enemy = 1
		* Spell = 2
		* Wall = 3
		* None = 4
		* Ensure type1 is always less than type2 for consistent handling
		*/
		CT type1 = event.type1;
		CT type2 = event.type2;
		if (type1 > type2)
		{
			std::swap(type1, type2);
			std::swap(event.entity1, event.entity2);
		}


		switch (type1) {
		case CT::Player:
			switch (type2) {
			case CT::Enemy:
				//Call resolve function for player vs enemy collision
				break;

			case CT::Spell:
				//Call resolve function for player vs spell collision
				break;

			case CT::Wall:
				//Call resolve function for player vs wall collision
				break;
			}
			break;

		case CT::Enemy:
			switch (type2) {
			case CT::Enemy:
				//Call resolve function for enemy vs enemy collision
				break;

			case CT::Spell:
				//Call function, maybe function handleSpellEffect() below
				break;

			case CT::Wall:
				std::cout << "Enemy collided with Wall" << std::endl;
				break;

			}
			break;

		case CT::Spell:
			switch (type2) {
			case CT::Wall:
				std::cout << "Spell collided with Wall" << std::endl;
				break;
			}
			break;

		case CT::None:
			//throw an error or log a warning
			break;
		}

	}

}

void CombatSystem::resolveEnemySpellCollision() {
	//rename later
	// indicate which pell hit which enemy
	// calulate dmg 
	// apply dmg to enemy
}

void CombatSystem::applyDamage(float dmg, entt::entity) {
	// Apply damage to entities based on collision and spell effects
}