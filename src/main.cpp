#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <entt/entt.hpp>

#include "systems/WindowManager.h"

#include "Utils/VectorMath.h"
#include "resources/EnemyLibrary.h"
#include "components/Enemy.h"
#include "control/commandManager.h"
#include "control/inputHandler.h"
#include "components/EntityTags.h"
#include "components/movementComponents.h"
#include "systems/MovementPipeline.h"
#include "resources/SpellLibrary.h"

const int FPS = 60;

using namespace std;

int main() {
	entt::registry registry;
	CommandManager commandManager(registry);

	EnemyLibrary enemyLibrary;
	SpellLibrary spellLibrary;

	MovementPipeline movementPipeline(spellLibrary, enemyLibrary);

	entt::entity playerEntity = registry.create();
	registry.emplace<PlayerTag>(playerEntity);
	registry.emplace<Position>(playerEntity, Position{ 100.0f, 100.0f });
	registry.emplace<Speed>(playerEntity, Speed{ 10.0f });

	sf::RectangleShape playerShape(sf::Vector2f(50.0f, 50.0f));


	InputHandler inputHandler(playerEntity, &commandManager);

	WindowManager& windowManager = WindowManager::getInstance();
	windowManager.createWindow(800, 600, "Window");
	
	while(windowManager.isOpen()) {
		while(const optional event = windowManager.pollEvent()) {
			if (event->is<sf::Event::Closed>())
			{
				windowManager.close();
			}
		}
		inputHandler.handleInput();
		commandManager.executeCommands();

		movementPipeline.update(registry, 1.0/FPS);

		windowManager.clear(sf::Color::Black);
		Position& playerPos = registry.get<Position>(playerEntity);
		playerShape.setPosition(playerPos);
		windowManager.draw(playerShape);
		windowManager.display();
	}

}