#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <filesystem>

#include "ui/UIManager.h"
#include "systems/WindowManager.h"
#include "ui/Button.h"
#include "ui/Text.h"
#include "ui/Panel.h"

const int FPS = 60;

using namespace std;

int main() {
	try {
		WindowManager& windowManager = WindowManager::getInstance();
		windowManager.createWindow(800, 600, "Test UI");
		UICommandManager& uiCommandManager = UICommandManager::getInstance();
		sf::RenderWindow& window = windowManager.getWindow();

		sf::Font font("src/resources/test.ttf");
		UIManager uiManager;
		
		Panel* panel = new Panel({ 50.f, 50.f }, { 100.f, 100.f });
		Button* exitButton = new Button("exit", "Exit", &font, { 10.f, 10.f }, 30);
		exitButton->setBackgroundColor(sf::Color::White);
		panel->addElement(exitButton);
		panel->setDraggable(true);

		uiManager.addElement(panel);

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
			uiManager.syncUIWithViewport();
			uiManager.draw(window);
			window.display();
		}
	}
	catch (const std::exception& e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}