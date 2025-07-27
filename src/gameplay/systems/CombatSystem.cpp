#include "CombatSystem.h"
#include "../components/StatComponent.h"
#include "../components/Behavior.h"
#include "../components/EntityTags.h"
#include "CollisionSystem.h"
#include "../../manager/SpellLibrary.h"
#include "../../manager/EnemyLibrary.h"

CombatSystem::CombatSystem(entt::registry& registry)
	: registry(registry)
{
	
}

void CombatSystem::handleEvent(const CollisionEvent& event)
{
	if (event.type1 == CollisionType::Player && event.type2 == CollisionType::Enemy)
	{
		handlePlayerEnemyCollision(event.entity1, event.entity2);
		applyDamage(event.entity1, event.entity2);
	}
	else if (event.type1 == CollisionType::Enemy && event.type2 == CollisionType::Spell)
	{
		handleEnemySpellCollision(event.entity1, event.entity2);
		applyDamage(event.entity1, event.entity2);
		if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
			(*dispatcher)->enqueue<SpellReduction>(SpellReduction{ event.entity2 });
		} else {
			std::cerr << "Dispatcher not found in registry context." << std::endl;
		}
	}
}


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

	// Optionally trigger death animation using dispatcher
	//if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
	//	 (*dispatcher)->enqueue<EnemyDeathEvent>(enemy);
	//}
}

void CombatSystem::applyDamage(entt::entity entityA, entt::entity entityB) 
{
	if (registry.all_of<EnemyTag>(entityB)) {
		float& attack = registry.get<Attack>(entityB).value;
		float& health = registry.get<Health>(entityA).current;
		health -= attack;
	}
	if (registry.all_of<SpellTag>(entityB)) {
		float& attack = registry.get<Attack>(entityB).value;
		float& health = registry.get<Health>(entityA).current;
		health -= attack;
	}
}

void CombatSystem::sinkEvents() 
{
	if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
		(*dispatcher)->sink<CollisionEvent>().connect<&CombatSystem::handleEvent>(*this);
	} else {
		std::cerr << "Dispatcher not found in registry context." << std::endl;
	}
}