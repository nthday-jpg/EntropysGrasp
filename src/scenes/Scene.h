#pragma once
#include <optional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>  // Add this
#include "../manager/WindowManager.h"
#include "../ui/UIManager.h"

class Scene {
protected:
	UIManager* uiManager;
	sf::RenderWindow& window;
	entt::dispatcher* dispatcher = nullptr;  // Add dispatcher pointer

	bool isActive = false;
	bool isLoaded = false;
public:
	Scene() = delete;

	Scene(sf::RenderWindow& window);

	~Scene()
	{
		if (uiManager) {
			delete uiManager;
			uiManager = nullptr;
		}
	}
	
	// Add setter for dispatcher
	void setDispatcher(entt::dispatcher* dispatcher) { this->dispatcher = dispatcher; }
	
	virtual void load() = 0;
	virtual void unload() = 0;
	virtual bool handleEvent(const std::optional <sf::Event>& event) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
	//virtual void activate() = 0;
	//virtual void deactivate() = 0;
	bool getActive() const { return isActive; }
	void display();
};