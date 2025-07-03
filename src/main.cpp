#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <filesystem>

#include "ui/UIManager.h"
#include "systems/WindowManager.h"
#include "ui/Button.h"

const int FPS = 60;

using namespace std;

int main() {
	WindowManager& windowManager = WindowManager::getInstance();
	windowManager.createWindow(800,600, "Test UI");
	UICommandManager& uiCommandManager = UICommandManager::getInstance();
	sf::RenderWindow& window = windowManager.getWindow();

	sf::Font font("src/resources/test.ttf");
	UIManager uiManager;
	uiManager.addButton(
		new Button(
			"pause",
			"Pause",
			&font,
			sf::Vector2f(100, 100),
			30
		)
	);


	while (window.isOpen())
	{
		window.clear(sf::Color::Black);
		while (const optional event = WindowManager::getInstance().pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				WindowManager::getInstance().close();
			}
			uiManager.handleEvent(*event);
		}
		uiCommandManager.executeCommands();
		uiManager.draw(window);
		window.display();
	}

}