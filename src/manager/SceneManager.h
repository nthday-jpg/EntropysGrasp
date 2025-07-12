#pragma once
#include "../scenes/Scene.h"
#include <unordered_map>
#include <string>
#include <optional>
#include <SFML/Window/Event.hpp>
#include <entt/signal/dispatcher.hpp>  // Add this include

class SceneManager
{
	// Use dynamic casting when use scene in scenes to ensure type safety
	std::unordered_map<std::string, Scene*> scenes;

	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;

	entt::dispatcher* dispatcher = nullptr;  // Add dispatcher pointer

	//implement transition between scenes
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

	// Add method to bind dispatcher
	void bindDispatcher(entt::dispatcher* dispatcher);

	void addScene(std::string name, Scene* scene);
	//load and unload scenes

	void navigateTo(std::string name);

	Scene* getCurrentScene() const { return currentScene; }

	void update(float deltaTime);

	void render();

	// handle gameflow events
	bool handleEvent(const std::optional<sf::Event>& event);
	
	// Add method to get dispatcher for scenes
	entt::dispatcher* getDispatcher() const { return dispatcher; }
};