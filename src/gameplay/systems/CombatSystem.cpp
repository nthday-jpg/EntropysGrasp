#include "CombatSystem.h"
#include "../components/StatComponent.h"
#include "../components/Behavior.h"

void CombatSystem::handlePlayerEnemyCollision(entt::entity player, entt::entity enemy) 
{
	auto& playerHealth = registry.get<Health>(player);
	auto& enemyHealth = registry.get<Health>(enemy);
	// Example: Player deals damage to enemy
	float damage = 10.0f; // Example damage value
	playerHealth.current -= damage * 0.1f;
	if (playerHealth.current <= 0)
	{
		//exit the game or respawn player
	}
}

void CombatSystem::handleEnemySpellCollision(entt::entity enemy, entt::entity spell) 
{
	auto& EnemyHealth = registry.get<Health>(enemy);
	// Example: Player takes damage from spell
	SpellID spellName = registry.get<SpellID>(spell);
	const SpellLibrary& spellLibrary = SpellLibrary::getInstance(); // Assuming SpellLibrary is a singleton
	const SpellData& spellData = spellLibrary.getSpell(spellName); // Assuming getSpellData is a function that retrieves spell data
	auto damage = spellData.damage; // Assuming SpellData has a damage field
	EnemyHealth.current -= damage;
	if (EnemyHealth.current <= 0) 
	{
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

void CombatSystem::applyDamage(float dmg, entt::entity) 
{
	// Apply damage to entities based on collision and spell effects
}