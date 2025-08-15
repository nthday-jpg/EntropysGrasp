#include "SceneManager.h"
#include "../scenes/MainMenu/MainMenu.h"
#include "../scenes/Transition/Transition.h"
#include "../manager/WindowManager.h"
#include "../scenes/Gameplay/GameplayScene.h"

SceneManager::SceneManager()
{
	addScene("MainMenu", new MainMenu(WindowManager::getInstance().getWindow()));
}

void SceneManager::bindDispatcher(entt::dispatcher* dispatcher)
{
	this->dispatcher = dispatcher;
	for (auto& [name, scene] : scenes) {
		scene->setDispatcher(dispatcher);
	}
}

void SceneManager::addScene(std::string name, Scene* scene) 
{
	scenes[name] = scene;
	if (dispatcher) 
	{
		scene->setDispatcher(dispatcher);
	}
	if (currentScene == nullptr) 
	{
		currentScene = scene;
		// Load the scene when it becomes the current scene
		currentScene->load();
	}
}

void SceneManager::navigateTo(const std::string& name) {
	// Call transitionTo with default wait duration of 2.0f
	auto it = scenes.find(name);
	if (it == scenes.end())
	{
		if (name == "Gameplay") 
		{
			addScene(name, new GameplayScene(WindowManager::getInstance().getWindow(), dispatcher));
		}
		it = scenes.find(name);
	}
	transitionTo(name, 4.0f);
}

void SceneManager::navigateToImmediate(const std::string& name) 
{
	// Direct navigation without transition (for internal use by TransitionScene)
	auto it = scenes.find(name);
	if (it == scenes.end()) 
	{
		if (name == "Gameplay")
		{
			addScene(name, new GameplayScene(WindowManager::getInstance().getWindow(), dispatcher));
		}
		it = scenes.find(name);
	}
	
	if (it != scenes.end()) 
	{
		// Unload current scene before switching
		if (currentScene) {
			currentScene->unload();
		}
		
		currentScene = it->second;
		// Load the new current scene
		currentScene->load();
	}
}

void SceneManager::transitionTo(const std::string& targetScene, float waitDuration) 
{
	// Create a unique transition scene name to avoid conflicts
	std::string transitionName = "Transition_" + targetScene;
	
	// Remove any existing transition scene
	auto transitionIt = scenes.find(transitionName);
	if (transitionIt != scenes.end()) {
		delete transitionIt->second;
		scenes.erase(transitionIt);
	}
	
	// Unload current scene before switching to transition
	if (currentScene) {
		currentScene->unload();
	}
	
	// Create and add new transition scene
	TransitionScene* transition = new TransitionScene(
		WindowManager::getInstance().getWindow(), 
		targetScene, 
		waitDuration
	);
	
	addScene(transitionName, transition);
	currentScene = transition;
	// Load the transition scene
	currentScene->load();
}

void SceneManager::update(float deltaTime) 
{
	if (currentScene) 
	{
		currentScene->update(deltaTime);
	}
}

void SceneManager::render()
{
	if (currentScene) 
	{
		currentScene->render();	
	}
}

bool SceneManager::handleEvent(const std::optional<sf::Event>& event) {
	if (currentScene) {
		return currentScene->handleEvent(event);
	}
	return false;
}