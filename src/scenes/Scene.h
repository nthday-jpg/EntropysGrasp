#pragma once
#include <optional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <entt/entity/registry.hpp>
#include "../systems/WindowManager.h"
#include "../ui/UIManager.h"

class Scene {
protected:
	UIManager* uiManager;
	sf::RenderWindow& window;

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
	virtual void load() = 0;
	//virtual void unload() = 0;
	virtual bool handleEvent(const std::optional <sf::Event>& event) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
	//virtual void activate() = 0;
	//virtual void deactivate() = 0;
	bool getActive() const { return isActive; }
	void display();
};