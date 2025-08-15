#include "Game.h"
#include "manager/FontManager.h"
#include "manager/TextureManager.h"
#include "manager/EnemyLibrary.h"
#include "manager/SpellLibrary.h"
#include <iostream>

using namespace sf;
using namespace std;

Game::Game() :
	windowManager(&WindowManager::getInstance())
{
	int height = gameConfig.getWindowHeight();
	int width = gameConfig.getWindowWidth();
	int FPS = gameConfig.maxFPS;

	windowManager->createWindow(width, height, "Game");
	windowManager->setFramerateLimit(FPS);

	sceneManager = &SceneManager::getInstance();
	uiCommandManager = &UICommandManager::getInstance();

	// Initialize core resource managers early
	// initializeResourceManagers();

	// Bind dispatcher to both managers
	SoundManager::getInstance().bindDispatcher(&dispatcher);
	sceneManager->bindDispatcher(&dispatcher);
}

void Game::initializeResourceManagers()
{
	try {
		// Initialize font manager (loads default fonts)
		FontManager::getInstance();
		std::cout << "FontManager initialized successfully" << std::endl;

		// Initialize texture manager and load all textures
		TextureManager& textureManager = TextureManager::getInstance();
		if (!textureManager.loadFromAssetFile()) {
			throw std::runtime_error("Failed to load textures");
		}
		std::cout << "TextureManager initialized successfully" << std::endl;

		// Initialize spell library (loads spell data)
		SpellLibrary::getInstance();
		std::cout << "SpellLibrary initialized successfully" << std::endl;

		// Initialize enemy library (loads enemy data)
		EnemyLibrary::getInstance();
		std::cout << "EnemyLibrary initialized successfully" << std::endl;

	} catch (const std::exception& e) {
		std::cerr << "Failed to initialize resource managers: " << e.what() << std::endl;
		throw; // Re-throw to prevent the game from starting with missing resources
	}
}

void Game::run()
{
	while (windowManager->isOpen())
	{
		float deltaTime = gameConfig.timeStep; 
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