#include <SFML/Graphics.hpp>
#include <iostream>
#include <entt/entt.hpp>
#include "systems/CollisionSystem.h"
#include "systems/MovementSystem.h"
#include "components/movementComponents.h"
#include "components/EntityTags.h"
#include "components/lookingDirection.h"
#include "components/statComponent.h"
#include "components/hitbox.h"
#include "behaviors/PlayerMovementSystem.h"
#include "resources/SpellLibrary.h"
#include "components/Spell.h"
#include "systems/BehaviorSystem.h"
#include "systems/SpellSystem.h"

using namespace std;
using namespace sf;

void normalize(Vector2f& vec) {
	float length = sqrt(vec.x * vec.x + vec.y * vec.y);
	if (length > 0) {
		vec.x /= length;
		vec.y /= length;
	}
}

int main() {
	RenderWindow window(VideoMode({ 800, 600 }), "Game Test");

	entt::registry registry;
	CollisionSystem collisionSystem(registry);
	MovementSystem movementSystem;
	
	EnemyLibrary enemyLibrary;
	SpellLibrary spellLibrary;
	SpellData spellData = { 1, 1, 1, 1, 50, 1, 3, 1, SpellEffect::Burn, BehaviorType::Straight };
	spellLibrary.spellDatabase[SpellID::Fireball] =  spellData;

	BehaviorSystem behaviorSystem;
	SpellSystem spellSystem;


	Hitbox pHitbox(50.0f, 50.0f, 0.0f, 0.0f);

	auto player = registry.create();
	registry.emplace<PlayerTag>(player);
	registry.emplace<Position>(player, 0.0f, 0.0f);
	registry.emplace<Speed>(player, 20.0f);
	registry.emplace<Hitbox>(player, pHitbox);
	registry.emplace<MovementDirection>(player, 0.0f, 0.0f);
	registry.emplace<LookingDirection>(player, 0.0f, 0.0f);
	registry.emplace<Mana>(player, 100.0f);

	RectangleShape playerShape(Vector2f(50.0f, 50.0f));
	playerShape.setFillColor(Color::Green);
	playerShape.setPosition({ 0.0f, 0.0f });

	RectangleShape spellShape(Vector2f(15.0f, 15.0f));
	spellShape.setFillColor(Color::Red);

	spellSystem.castTimes[SpellID::Fireball] = 0.5f;
	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		while (const optional event = window.pollEvent())
		{
			if (event->is<Event::Closed>())
			{
				window.close();
			}
		}


		MovementDirection& moveDir = registry.get<MovementDirection>(player);
		moveDir = { 0.0f, 0.0f };


		if (Keyboard::isKeyPressed(Keyboard::Scancode::A))
		{
			moveDir.x = -1.0f;
			cout << "Moving Left" << endl;
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::D))
		{
			moveDir.x = 1.0f;
			cout << "Moving Right" << endl;
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::W))
		{
			moveDir.y = -1.0f;
			cout << "Moving Up" << endl;
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::S))
		{
			moveDir.y = 1.0f;
			cout << "Moving Down" << endl;
		}
		normalize(moveDir);


		LookingDirection& lookDir = registry.get<LookingDirection>(player);
		Position& playerPos = registry.get<Position>(player);
		lookDir = { 10,10 };
		normalize(lookDir);


		PlayerMovementSystem::calculateVelo(registry);
		movementSystem.update(registry, 1.0f / 60.0f);
		Hitbox& playerHitbox = registry.get<Hitbox>(player);
		playerShape.setPosition({ playerPos.x ,playerPos.y });


		behaviorSystem.initializeBehaviorMap();

		behaviorSystem.updateBehavior(registry, 1.0f / 60.0f, spellLibrary, enemyLibrary);

		spellSystem.update(registry, 1.0f / 60.0f, spellLibrary);


		auto view = registry.view<Position, SpellTag>();
		for (auto [entity, position] : view.each())
		{
			cout << "Spell Position: " << position.x << ", " << position.y << endl;
			spellShape.setPosition({ position.x, position.y });

		}

		window.clear(Color::Black);
		window.draw(spellShape);
		window.draw(playerShape);
		window.display();
	}

}