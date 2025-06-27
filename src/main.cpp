#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <entt/entt.hpp>

#include "Utils/VectorMath.h"
#include "resources/EnemyLibrary.h"
#include "components/Enemy.h"
#include "control/commandManager.h"
#include "control/inputHandler.h"
#include "components/EntityTags.h"
#include "components/movementComponents.h"
#include "systems/MovementSystem.h"
#include "behaviors/PlayerMovementSystem.h"

using namespace std;

int main() {
	EnemyLibrary enemyLibrary;
	entt::registry registry;
	CommandManager commandManager(registry);

	MovementSystem movementSystem;
	PlayerMovementSystem playerMovement;

	entt::entity playerEntity = registry.create();
	registry.emplace<PlayerTag>(playerEntity);
	registry.emplace<Position>(playerEntity, Position{ 100.0f, 100.0f });
	registry.emplace<Speed>(playerEntity, Speed{ 1000.0f });

	sf::RectangleShape playerShape(sf::Vector2f(50.0f, 50.0f));


	InputHandler inputHandler(playerEntity, &commandManager);


	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Game Window");
	window.setFramerateLimit(10);
	while(window.isOpen()) {
		while(const std::optional event = window.pollEvent()) {
			if(event->is<sf::Event::Closed>()) {
				window.close();
			}
		}
		inputHandler.handleInput();
		commandManager.executeCommands();
		playerMovement.calculateVelo(registry);
		movementSystem.update(registry, 0.016f);

		window.clear(sf::Color::Black);
		Position& playerPos = registry.get<Position>(playerEntity);
		playerShape.setPosition(playerPos);
		window.draw(playerShape);
		window.display();
	}

}