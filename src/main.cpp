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
#include "resources/SpellLibrary.h"4
#include "resources/EnemyLibrary.h"
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
	RenderWindow window(VideoMode({ 800, 600 }), "ECS Example");

	entt::registry registry;
	CollisionSystem collisionSystem(registry);
	MovementSystem movementSystem;
	
	EnemyLibrary enemyLibrary;
	SpellLibrary spellLibrary;
	SpellData spellData = { 1, 1, 1, 1, 100, 1, 50, 40, SpellEffect::Burn, BehaviorType::HomingEnemy };
	spellLibrary.spellDatabase[SpellID::Fireball] = spellData;

	BehaviorSystem behaviorSystem;
	SpellSystem spellSystem;

	Hitbox pHitbox(50.0f, 50.0f, 0.0f, 0.0f);

	auto player = registry.create();
	registry.emplace<PlayerTag>(player);
	registry.emplace<Position>(player, 0.0f, 0.0f);
	registry.emplace<Speed>(player, 200.0f);
	registry.emplace<Hitbox>(player, pHitbox);
	registry.emplace<MovementDirection>(player, 0.0f, 0.0f);
	registry.emplace<LookingDirection>(player, 0.0f, 0.0f);
	registry.emplace<Mana>(player, 100.0f);
	registry.emplace<Resistance>(player, 0.5f);

	RectangleShape playerShape(Vector2f(50.0f, 50.0f));
	playerShape.setFillColor(Color::Green);
	playerShape.setPosition({ 0.0f, 0.0f });

	EnemyData enemyData = { 100, 100, 100, 0, 100, 0.3f, BehaviorType::HomingPlayer };
	enemyLibrary.enemyDatabase[EnemyType::Dragon] = enemyData;

	auto enemy = registry.create();
	registry.emplace<EnemyTag>(enemy);
	registry.emplace<EnemyType>(enemy, EnemyType::Dragon);
	registry.emplace<Position>(enemy, 100.0f, 100.0f);
	registry.emplace<Speed>(enemy, enemyData.speed.value);
	registry.emplace<Hitbox>(enemy, Hitbox(50.0f, 50.0f, 0.0f, 0.0f));
	registry.emplace<MovementDirection>(enemy, 0.0f, 0.0f);
	registry.emplace<Resistance>(enemy, enemyData.resistance.value);
	registry.emplace<BehaviorType>(enemy, enemyData.behaviorType);

	RectangleShape enemyShape(Vector2f(50.0f, 50.0f));
	enemyShape.setFillColor(Color::Red);


	RectangleShape spellShape(Vector2f(15.0f, 15.0f));
	spellShape.setFillColor(Color::Red);
	
	behaviorSystem.initializeBehaviorMap();

	spellSystem.castTimes[SpellID::Fireball] = 0.5f;
	window.setFramerateLimit(60);
	bool cast = false;
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
			moveDir.x += -1.0f;
			cout << "Moving Left" << endl;
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::D))
		{
			moveDir.x += 1.0f;
			cout << "Moving Right" << endl;
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::W))
		{
			moveDir.y += -1.0f;
			cout << "Moving Up" << endl;
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::S))
		{
			moveDir.y += 1.0f;
			cout << "Moving Down" << endl;
		}
		if (Keyboard::isKeyPressed(Keyboard::Scancode::F)) {
			cast = true;
		}
		normalize(moveDir);

		if (cast) 
		{
			behaviorSystem.updateBehavior(registry, 1.0f / 60.0f, spellLibrary, enemyLibrary);
			spellSystem.update(registry, 1.0f / 60.0f, spellLibrary);
		}



		LookingDirection& lookDir = registry.get<LookingDirection>(player);
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		Position& playerPos = registry.get<Position>(player);
		lookDir = { mousePos.x - playerPos.x, mousePos.y - playerPos.y };
		normalize(lookDir);

		PlayerMovementSystem::calculateVelo(registry);
		movementSystem.update(registry, 1.0f / 60.0f);
		playerShape.setPosition({ playerPos.x ,playerPos.y });

		Position& enemyPos = registry.get<Position>(enemy);
		enemyShape.setPosition({ enemyPos.x, enemyPos.y });


		collisionSystem.detectCollisions();
		for (const auto& event : collisionSystem.getCollisionEvents())
		{
			collisionSystem.resolvePhysicalOverlap(event.entity1, event.entity2);
		}

		auto view = registry.view<Position, SpellTag>();
		for (auto [entity, position] : view.each())
		{
			cout << "Spell Position: (" << position.x << ", " << position.y << ")" << endl;
			spellShape.setPosition({ position.x, position.y });
		}


		window.clear(Color::Black);
		window.draw(spellShape);
		window.draw(enemyShape);
		window.draw(playerShape);
		window.display();
	}

}