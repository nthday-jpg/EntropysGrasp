#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <filesystem>

#include "ui/UIManager.h"
#include "systems/WindowManager.h"
#include "ui/Button.h"
#include "ui/Text.h"
#include "ui/Panel.h"
#include "scenes/MainMenu/MainMenu.h"

const int FPS = 10;

using namespace std;

int main() {
	try {
		WindowManager& windowManager = WindowManager::getInstance();
		windowManager.createWindow(800, 600, "Test UI");

		UICommandManager& uiCommandManager = UICommandManager::getInstance();
		sf::RenderWindow& window = windowManager.getWindow();
		sf::View view(sf::FloatRect({ 0.0f, 0.0f }, { 800.0f, 600.0f }));
		window.setView(view);
		window.setFramerateLimit(FPS);

		MainMenu mainMenu(window);
		
		sf::RectangleShape shape(sf::Vector2f(200.f, 50.f));
		shape.setPosition({ 300.f, 200.f });

		while (window.isOpen())
		{
			window.clear(sf::Color::Black);
			

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
			{
				view.move({ 0.f, -5.f });
				cout << "Moving up" << endl;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
			{
				view.move({ -5.f, 0.f });
				cout << "Moving left" << endl;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S))
			{
				view.move({ 0.f, 5.f });
				cout << "Moving down" << endl;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
			{
				view.move({ 5.f, 0.f });
				cout << "Moving right" << endl;
			}

			window.setView(view);
			mainMenu.update(1.0f / FPS);
			uiCommandManager.executeCommands();
			window.draw(shape);
			//uiManager.draw(window);
			window.display();
		}
	}
	catch (const std::exception& e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}