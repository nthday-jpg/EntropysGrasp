#include "GameControl.h"
#include "../../scenes/Gameplay/GameplayScene.h"
#include <iostream>

void Pause::execute()
{
	SceneManager& sceneManager = SceneManager::getInstance();
	if (auto currentScene = dynamic_cast<GameplayScene*>(sceneManager.getCurrentScene())) {
		currentScene->pause();
		std::cout << "Game paused." << std::endl;
	} else {
		std::cerr << "No current scene to pause." << std::endl;
	}
}

void Restart::execute()
{
	// Xem lai cai nay sau
	SceneManager& sceneManager = SceneManager::getInstance();
	if (auto currentScene = dynamic_cast<GameplayScene*>(sceneManager.getCurrentScene())) {
		currentScene->restart(); // Reload the current scene
		std::cout << "Game restarted." << std::endl;
	} else {
		std::cerr << "No current scene to restart." << std::endl;
	}
}

void Exit::execute()
{
	SceneManager& sceneManager = SceneManager::getInstance();
	if (auto currentScene = dynamic_cast<GameplayScene*>(sceneManager.getCurrentScene())) {
		currentScene->exit(); // Logic to exit the game
		std::cout << "Game exited." << std::endl;
	} else {
		std::cerr << "No current scene to exit." << std::endl;
	}
}

void ChangeScene::execute()
{
	std::cout << "Changing scene to: " << targetScene << std::endl;
	sceneManager->navigateTo(targetScene);
}