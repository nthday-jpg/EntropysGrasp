#pragma once
#include "../Scene.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "../../ui/UIManager.h"

class MainMenu : public Scene
{
	// Inherited via Scene
	// RenderWindow* window;
	// UIManager* uiManager;
	sf::Font font;
public:
	MainMenu() = delete;

	MainMenu(sf::RenderWindow& window);

	bool handleEvent(const std::optional<sf::Event>& event) override;

	void update(float deltaTime) override;

	void load() override;

	void render() override;
};