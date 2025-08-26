#pragma once
#include <entt/entt.hpp>
#include <unordered_map>
#include <SFML/System/Clock.hpp>
#include "../../manager/EnemyLibrary.h"
#include "../components/MovementComponents.h"
#include "../components/EnemySpawnInfo.h"
#include "../../scenes/Gameplay/Camera.h"
#include "../components/statComponent.h"
#include "../components/Reward.h"


// This file defines the EnemyManager class which handles enemy spawning, updating,
// and removing in the game. 
// The EnemyManager interacts with an EnemyLibrary to retrieve enemy data and uses a clock to manage
// spawn timing. It also checks if enemies are within the load chunk to determine
// whether they should be spawned or removed. The class provides methods for updating
// the enemy state, spawning new enemies, and removing inactive or defeated enemies.


class EnemyManager
{
public:
	EnemyManager(entt::registry& registry, const Camera& camera, sf::Clock& gameClock);
	void update(float dt);
	void sinkEvents();

private:
	entt::registry& registry;
	const EnemyLibrary& enemyLibrary = EnemyLibrary::getInstance();
	const Camera& camera;
	const sf::Clock& gameClock;
	const float loadChunkRadius = 10000000.0f; //Here's square radius

	// spawn timers keep track of time since last spawn for each enemy type
	std::unordered_map<EnemyType, float> spawnTimer;

	// const MapManager to query spawn information
	const std::vector<EnemySpawnInfo> spawnInfos = {
		EnemySpawnInfo{EnemyType::Goblin, {Position(100, 100), Position(200, 200)}, 5.0f},
		EnemySpawnInfo{EnemyType::Orc, {Position(-100, -100), Position(-200, -200)}, 5.0f}
	};

	// Handle spawning of enemies based on spawn information and time delta
	// Check the conditions for spawning enemies
	// Call spawnEnemy for each position in the spawn info
	void spawning(float dt);

	// Handle removing enemies based on their health
	// If health is less than zero, or outside of load chunk, mark the enemy inactive
	void removing();

	// Create an enemy of the given type at the specified position in registry
	// Or reuse an inactive enemy if available
	entt::entity spawnEnemy(EnemyType type, Position position);

	// Return whether the position is in load chunk
	bool isInLoadChunk(const Position& position) const;

	// Method to spawn enemy randomly beyond the view
	Position randomizeOffScreenPosition(const Position& position) const;

	bool isPositionInScreen(const Position& position) const;

	int getBaseExp(EnemyType type);
	int getBaseGold(EnemyType type);
	Mana getBaseMana(EnemyType type);
};