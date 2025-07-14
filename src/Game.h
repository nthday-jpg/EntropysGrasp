#pragma once
#include <SFML/Graphics.hpp>
#include <entt/signal/dispatcher.hpp>
#include "manager/SceneManager.h"
#include "manager/WindowManager.h"
#include "control/UICommandManager.h"
#include "manager/SoundManager.h"


//Hằng số
const int windowHeight = 600;
const int windowWidth = 800;
const int FPS = 60;

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
	UICommandManager* uiCommandManager;
	SceneManager* sceneManager;

	entt::dispatcher dispatcher;

	sf::Clock clock;
	float deltaTime = 0.0f;

	void processEvents();
	void update(float deltaTime);
	void render();

};