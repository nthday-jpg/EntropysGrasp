#pragma once
#include <SFML/Graphics.hpp>
#include <entt/signal/dispatcher.hpp>
#include "manager/SceneManager.h"
#include "manager/WindowManager.h"
#include "control/UICommandManager.h"
#include "manager/SoundManager.h"


//Hằng số
constexpr int windowHeight = 600;
constexpr int windowWidth = 800;
constexpr int FPS = 60;
constexpr float deltaTime = 1.0f / FPS;


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

	float accumulator = 0.0f;
	sf::Clock clock;

	void processEvents();
	void update(float deltaTime);
	void render();

};