#include "playerControl.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "../../systems/WindowManager.h"
#include "../../components/movementComponents.h"
#include "../../components/lookingDirection.h"
#include "../../components/EntityTags.h"

#include "../../Utils/VectorMath.h"


const MovementDirection down = MovementDirection(0, 1);
const MovementDirection up = MovementDirection(0, -1);
const MovementDirection left = MovementDirection(-1, 0);
const MovementDirection right = MovementDirection(1, 0);

void MoveDown::execute(entt::registry& registry)
{
	if (!registry.all_of<MovementDirection, PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not have MovementDirection or PlayerTag component." << std::endl;
		return;
	}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += down;

	std::cout << "Moving down: " << std::endl;
}

void MoveUp::execute(entt::registry& registry)
{
	if (!registry.all_of<MovementDirection, PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not have MovementDirection or PlayerTag component." << std::endl;
		return;
	}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += up;

	std::cout << "Moving up: " << std::endl;
}

void MoveLeft::execute(entt::registry& registry)
{
	if (!registry.all_of<MovementDirection, PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not have MovementDirection or PlayerTag component." << std::endl;
		return;
	}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += left;

	std::cout << "Moving left: " << std::endl;
}

void MoveRight::execute(entt::registry& registry)
{
	if (!registry.all_of<MovementDirection, PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not have MovementDirection or PlayerTag component." << std::endl;
		return;
	}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += right;
	
	std::cout << "Moving right: " << std::endl;
}

void CastSpell::execute(entt::registry& registry)
{

}

void Dash::execute(entt::registry& registry)
{

}

void ResetTempComponents::execute(entt::registry& registry)
{
	if (!registry.all_of<PlayerTag>(playerEntity))
	{
		std::cerr << "Player entity does not PlayerTag component." << std::endl;
		return;
	}

	if (!registry.all_of<MovementDirection>(playerEntity))
	{
		registry.emplace<MovementDirection>(playerEntity, MovementDirection(0, 0));
	}
	else
	{
		registry.get<MovementDirection>(playerEntity) = MovementDirection(0, 0);
	}

	if (!registry.all_of<LookingDirection>(playerEntity))
	{
		registry.emplace<LookingDirection>(playerEntity, LookingDirection{ 0.0f,0.0f });
	}
	else
	{
		registry.get<LookingDirection>(playerEntity) = LookingDirection(0.0f, 0.0f);
	}

}

void LookAtMouse::execute(entt::registry& registry)
{
	if (!registry.all_of<LookingDirection, PlayerTag, Position>(playerEntity))
	{
		std::cerr << "Player entity does not have LookingDirection or PlayerTag or Position component." << std::endl;
		return;
	}

	WindowManager& windowManager = WindowManager::getInstance();
	if (!windowManager.hasWindow() || !windowManager.isOpen())
	{
		std::cerr << "Window is not open or does not exist." << std::endl;
		return;
	}

	Position& playerPosition = registry.get<Position>(playerEntity);
	sf::Vector2i mousePosition = sf::Mouse::getPosition(windowManager.getWindow());

	LookingDirection& lookingDirection = registry.get<LookingDirection>(playerEntity);
	lookingDirection = LookingDirection{
		static_cast<float>(mousePosition.x - playerPosition.x),
		static_cast<float>(mousePosition.y - playerPosition.y) };
	
	normalize(lookingDirection);
}
