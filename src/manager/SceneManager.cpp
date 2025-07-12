#include "SceneManager.h"
#include "../scenes/MainMenu/MainMenu.h"
#include "../manager/WindowManager.h"

SceneManager::SceneManager()
{
	addScene("MainMenu", new MainMenu(WindowManager::getInstance().getWindow()));
}

void SceneManager::bindDispatcher(entt::dispatcher* dispatcher) {
	this->dispatcher = dispatcher;
	// Pass dispatcher to existing scenes
	for (auto& [name, scene] : scenes) {
		scene->setDispatcher(dispatcher);
	}
}

void SceneManager::addScene(std::string name, Scene* scene) {
	scenes[name] = scene;
	// Pass dispatcher to new scene if available
	if (dispatcher) {
		scene->setDispatcher(dispatcher);
	}
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