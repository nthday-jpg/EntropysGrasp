#include "CombatSystem.h"
#include "../components/StatComponent.h"
#include "../components/Behavior.h"
#include "../components/EntityTags.h"
#include "CollisionSystem.h"
#include "../../manager/SpellLibrary.h"
#include "../../manager/EnemyLibrary.h"
#include "StateSystem.h"

CombatSystem::CombatSystem(entt::registry& registry)
	: registry(registry)
{}

void CombatSystem::handleEvent(const CollisionEvent& event)
{
	if (event.type1 == CollisionType::Player && event.type2 == CollisionType::Enemy)
	{
		handlePlayerEnemyCollision(event.entity1, event.entity2);
	}
	else if (event.type1 == CollisionType::Enemy && event.type2 == CollisionType::Spell)
	{
		handleEnemySpellCollision(event.entity1, event.entity2);
		if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
			(*dispatcher)->enqueue<SpellReduction>(SpellReduction{ event.entity2 });
		} else {
			std::cerr << "Dispatcher not found in registry context." << std::endl;
		}
	}
}


void CombatSystem::handlePlayerEnemyCollision(entt::entity player, entt::entity enemy) 
{
	Health& playerHealth = registry.get<Health>(player);
	float damage = registry.get<Attack>(enemy).value;
	// Example: Player deals damage to enemy
	playerHealth.current -= damage;
	if (entt::dispatcher* dispatcher = *registry.ctx().find<entt::dispatcher*>()) {
		dispatcher->enqueue<StateChangeEventState>({ enemy, EntityState::Attacking, 1.0f });
	}
	if (playerHealth.current <= 0)
	{
		exit(0); // Player dies, exit the game
	}
}

void CombatSystem::handleEnemySpellCollision(entt::entity enemy, entt::entity spell) 
{
	Health& EnemyHealth = registry.get<Health>(enemy);
	// Example: Player takes damage from spell
	SpellID spellName = registry.get<SpellID>(spell);
	float damage = SpellLibrary::getInstance().getSpell(spellName).damage; // Assuming SpellData has a damage field
	EnemyHealth.current -= damage;


	// Optionally trigger death animation using dispatcher
	//if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
	//	 (*dispatcher)->enqueue<EnemyDeathEvent>(enemy);
	//}
}

void CombatSystem::sinkEvents() 
{
	if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
		(*dispatcher)->sink<CollisionEvent>().connect<&CombatSystem::handleEvent>(*this);
	} else {
		std::cerr << "Dispatcher not found in registry context." << std::endl;
	}
}