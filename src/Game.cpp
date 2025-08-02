#include "Game.h"
#include <iostream>

using namespace sf;
using namespace std;

Game::Game() :
	windowManager(&WindowManager::getInstance())
{
	windowManager->createWindow(800, 600, "Game");
	windowManager->setFramerateLimit(60);

	sceneManager = &SceneManager::getInstance();
	uiCommandManager = &UICommandManager::getInstance();

	// Bind dispatcher to both managers
	SoundManager::getInstance().bindDispatcher(&dispatcher);
	sceneManager->bindDispatcher(&dispatcher);  // Add this line
}

void Game::run()
{
	while (windowManager->isOpen())
	{
		float frameTime = clock.restart().asSeconds();
		if (frameTime > 0.05f) 
			frameTime = 0.05f; // giới hạn khi lag nặng
		accumulator += frameTime;
		processEvents();
		while (accumulator >= deltaTime)
		{
			accumulator -= deltaTime;
			update(frameTime);
		}
		render();
	}
}

void Game::processEvents()
{
	while (const std::optional<Event> event = windowManager->pollEvent()) 
	{
		if (bool isProcessed = windowManager->handleEvent(event))
		{
			continue;
		}

		if (bool isProcessed = sceneManager->handleEvent(event))
		{
			continue;
		}
	}
}

void Game::update(float deltatime)
{
	sceneManager->update(deltatime);
	uiCommandManager->executeCommands();
}

void Game::render()
{
	windowManager->clear();
	sceneManager->render();
	windowManager->display();
}