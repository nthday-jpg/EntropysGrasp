#include <iostream>
#include <SFML/Graphics.hpp>
#include "src/systems/CollisionSystem.h"
#include "src/systems/CombatSystem.h"
#include "src/components/statComponent.h"

using namespace std;
using namespace sf;

int main() {
	RenderWindow window(VideoMode({ 800, 600 }), "Game");
	entt::registry registry;
	entt::dispatcher dispatcher;

	CollisionSystem collisionSystem(registry, dispatcher);
	CombatSystem combatSystem(registry, dispatcher);

	entt::entity player = registry.create();
	registry.emplace<Position>(player, 100.0f, 100.0f);
	registry.emplace<Hitbox>(player, Hitbox(10,10,100,100)); 
	registry.emplace<ResistanceComponent>(player, ResistanceComponent{ 0.5f });
	
	entt::entity enemy = registry.create();
	registry.emplace<Position>(enemy, 200.0f, 200.0f);
	registry.emplace<Hitbox>(enemy, Hitbox(10, 10, 200, 200));
	registry.emplace<ResistanceComponent>(enemy, ResistanceComponent{ 0.5f });

	RectangleShape playerShape(Vector2f(10, 10));
	playerShape.setFillColor(Color::Green);
	Position& playerPos = registry.get<Position>(player);
	playerShape.setPosition(playerPos);

	Position& enemyPos = registry.get<Position>(enemy);
	RectangleShape enemyShape(Vector2f(10, 10));
	enemyShape.setFillColor(Color::Red);
	enemyShape.setPosition(enemyPos);

	Hitbox& playerHitbox = registry.get<Hitbox>(player);
	Hitbox& enemyHitbox = registry.get<Hitbox>(enemy);

	
	while (window.isOpen())
	{
		while (const optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Scancode::W)) {
			playerPos.y -= 0.05f;
			playerHitbox.offsetY -= 0.05f; // Move up
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::S)) {
			playerPos.y += 0.05f; // Move down
			playerHitbox.offsetY += 0.05f; // Move down
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::A)) {
			playerPos.x -= 0.05f; // Move left
			playerHitbox.offsetX -= 0.05f; // Move left
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::D)) {
			playerPos.x += 0.05f; // Move right
			playerHitbox.offsetX += 0.05f; // Move right
		}

		window.clear(Color::Black);
		collisionSystem.resolvePhysicalOverlap(player, enemy);

		playerShape.setPosition(Vector2f(playerHitbox.offsetX, playerHitbox.offsetY));

		enemyShape.setPosition(Vector2f(enemyHitbox.offsetX, enemyHitbox.offsetY));

		window.draw(playerShape);
		window.draw(enemyShape);
		window.display();

	}

}