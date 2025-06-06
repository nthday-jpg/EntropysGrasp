#include "CombatSystem.h"
#include "../components/statComponent.h"
#include "../components/Spell.h"

void CombatSystem::handlePlayerEnemyCollision(entt::entity player, entt::entity enemy) {
	auto& playerHealth = registry.get<HealthComponent>(player);
	auto& enemyHealth = registry.get<HealthComponent>(enemy);
	// Example: Player deals damage to enemy
	float damage = 10.0f; // Example damage value
	playerHealth.health -= damage * 0.1f;
	if (playerHealth.health <= 0) {
		//exit the game or respawn player
	}
}

void CombatSystem::handleEnemySpellCollision(entt::entity enemy, entt::entity spell, SpellLibrary spellLibrary) {
	auto& EnemyHealth = registry.get<HealthComponent>(enemy);
	// Example: Player takes damage from spell
	std::string spellName = registry.get<OnActivateSpell>(spell).spellID;
	SpellData spellData = spellLibrary.getSpell(spellName); // Assuming getSpellData is a function that retrieves spell data
	auto damage = spellData.damage; // Assuming SpellData has a damage field
	EnemyHealth.health -= damage;
	if (EnemyHealth.health <= 0) {
		// Handle enemy death, e.g., remove entity or trigger death animation
		registry.destroy(enemy); // Example of removing the enemy entity
	}
	SpellEffect spellType = spellData.effect;
	//if (spellType == "fireball") {
	//	std::cout << "Enemy hit by fireball spell!" << std::endl;
	// 	registry.emplace_or_replace<EffectTag>(enemy);
	//}
	//else if (spellType == "iceblast") {
	//	std::cout << "Enemy hit by ice blast spell!" << std::endl;
	//	registry.emplace_or_replace<EffectTag>(enemy);
	//}
	//else if (spellType == "lightningstrike") {
	//	std::cout << "Enemy hit by lightning strike spell!" << std::endl;
	//	registry.emplace_or_replace<EffectTag>(enemy);
	//}
	//else {
	//	std::cout << "Unknown spell type!" << std::endl;
	//}
}

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
	// rename later
	// indicate which pell hit which enemy
	// calulate dmg 
	// apply dmg to enemy
}

void CombatSystem::applyDamage(float dmg, entt::entity) {
	// Apply damage to entities based on collision and spell effects
}