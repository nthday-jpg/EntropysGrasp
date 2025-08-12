#pragma once
#include "../scenes/Scene.h"
#include <unordered_map>
#include <string>
#include <optional>
#include <SFML/Window/Event.hpp>
#include <entt/signal/dispatcher.hpp>

class SceneManager
{
	std::unordered_map<std::string, Scene*> scenes;

	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;

	entt::dispatcher* dispatcher;

	SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
public:
	
	static SceneManager& getInstance() {
		static SceneManager instance;
		return instance;
	}

	~SceneManager() {
		for (auto& pair : scenes) {
			delete pair.second;
		}
		scenes.clear();
	}

	void bindDispatcher(entt::dispatcher* dispatcher);
	void addScene(std::string name, Scene* scene);
	
	// Now calls transitionTo with default wait duration
	void navigateTo(const std::string& name);
	
	// Direct navigation without transition (for internal use)
	void navigateToImmediate(const std::string& name);
	
	// Transition with custom wait duration
	void transitionTo(const std::string& targetScene, float waitDuration = 2.0f);

	Scene* getCurrentScene() const { return currentScene; }

	void update(float deltaTime);
	void render();
	bool handleEvent(const std::optional<sf::Event>& event);
	entt::dispatcher* getDispatcher() const { return dispatcher; }
};