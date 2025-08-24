#pragma once
#include <SFML/Graphics.hpp>
#include <entt/signal/dispatcher.hpp>
#include "manager/SceneManager.h"
#include "manager/WindowManager.h"
#include "control/GameCommandManager.h"
#include "manager/SoundManager.h"
#include "GameConfig.h"

class Game {
public:
	void run();

	static Game& getInstance()
	{
		static Game instance;
		return instance;
	}

private:
	Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	~Game() = default;

	WindowManager* windowManager;
	GameCommandManager* gameCommandManager;
	SceneManager* sceneManager;
	GameConfig& gameConfig = GameConfig::getInstance();

	entt::dispatcher dispatcher;

	float accumulator = 0.0f;
	sf::Clock clock;

	void initializeResourceManagers();
	void processEvents();
	void update(float deltaTime);
	void render();
};