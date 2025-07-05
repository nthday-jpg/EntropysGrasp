#include "MainMenu.h"
#include "../../ui/Button.h"
#include "../../ui/Text.h"
#include "../../ui/Panel.h"
#include <SFML/Graphics/Font.hpp>

MainMenu::MainMenu(sf::RenderWindow& window) : Scene(window) , font("src/resources/test.ttf")
{
	isLoaded = false;
}

void MainMenu::update(float deltaTime) {
	if (!isLoaded) {
		load(); // Ensure resources are loaded before updating
	}
	uiManager->syncUIWithViewport();
	while(const std::optional event = window.pollEvent())
	{
		uiManager->handleEvent(event);
	}
	uiManager->draw(window);
}

void MainMenu::load()
{
	if (!isLoaded) {
		// Load resources, initialize variables, etc.
		// This is called once when the scene is loaded

		uiManager = new UIManager();
		uiManager->addElement(
			new Button(
				"PAUSE",
				"Pause",
				&font,
				{ 100,100 },
				30.0f
			)
		);

		isLoaded = true;
	}
}