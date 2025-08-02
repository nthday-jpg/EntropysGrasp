#pragma once
#include "../Scene.h"
#include "../../control/GameplayInputHandler.h"
#include "../../control/GameplayCommandManager.h"
#include "../../gameplay/systems/CollisionSystem.h"
#include "../../gameplay/systems/MovementPipeline.h"
#include "../../gameplay/systems/CombatSystem.h"
#include "../../gameplay/systems/SpellManager.h"
#include "../../gameplay/systems/EnemyManager.h"
#include "../../gameplay/systems/PhysicsSystem.h"
#include "../../gameplay/systems/RenderSystem.h"
#include "../../gameplay/systems/AnimationSystem.h"
#include "Camera.h"
#include "../../gameplay/systems/ParticleSystem.h"
#include <entt/entity/registry.hpp>
#include "../../gameplay/manager/MapManager.h"

class GameplayScene : public Scene {
private:
	//Mapmanager* mapManager;
    GameplayCommandManager* gameplayCommandManager;
    GameplayInputHandler* inputHandler;
	UIManager* pausedUI;

	sf::Clock gameClock;
	Camera camera;

    entt::registry registry;
    entt::dispatcher* dispatcher;
    
    // Systems
    CollisionSystem collisionSystem;
    MovementPipeline movementPipeline;
    CombatSystem combatSystem;
    SpellManager spellManager;
    EnemyManager enemyManager;
    PhysicsSystem physicsSystem;
    RenderSystem renderSystem;
	ParticleSystem particleSystem;
	AnimationSystem animationSystem;

    // UI
	bool isLoaded = false;
    
	bool isPaused = false;
    
public:
    GameplayScene(sf::RenderWindow& window, entt::dispatcher* dispatcher);
    ~GameplayScene();
    
    void load() override;
	void unload() override;
    bool handleEvent(const std::optional<sf::Event>& event) override;
    void update(float deltaTime) override;
    void render() override;

	void pause();
    void resume();
	void restart();
	void exit();
    void end();
    
private:
	entt::entity createPlayer();
};