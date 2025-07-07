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
		load(); 
		isLoaded = true; 
	}
	uiManager->syncUIWithViewport();
}

bool MainMenu::handleEvent(const std::optional<sf::Event>& event)
{
	if (!uiManager)
	{
		return false;
	}
	return uiManager->handleEvent(event);
}

void MainMenu::load()
{
	if (!isLoaded) {
		// Load resources, initialize variables, etc.
		// This is called once when the scene is loaded
		if (uiManager)
		{
			delete uiManager;
		}

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

void MainMenu::render()
{
	uiManager->draw(window);
}