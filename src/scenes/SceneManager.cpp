#include "SceneManager.h"
#include "MainMenu/MainMenu.h"
#include "../manager/WindowManager.h"

SceneManager::SceneManager()
{
	addScene("MainMenu", new MainMenu(WindowManager::getInstance().getWindow()));
}

void SceneManager::addScene(std::string name, Scene* scene) {
	scenes[name] = scene;
	if (currentScene == nullptr) {
		currentScene = scene; // Set the first scene as the current scene
	}
}

void SceneManager::navigateTo(std::string name) {
	auto it = scenes.find(name);
	if (it != scenes.end()) {
		currentScene = it->second;
	}
}

void SceneManager::update(float deltaTime) {
	if (currentScene) {
		currentScene->update(deltaTime);
	}
}

void SceneManager::render() {
	if (currentScene) {
		currentScene->render();	
	}
}

bool SceneManager::handleEvent(const std::optional<sf::Event>& event) {
	if (currentScene) {
		return currentScene->handleEvent(event);
	}
	return false;
}