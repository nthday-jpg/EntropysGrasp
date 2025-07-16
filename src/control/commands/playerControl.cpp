#include "PlayerControl.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "../../manager/WindowManager.h"
#include "../../gameplay/components/MovementComponents.h"
#include "../../gameplay/components/LookingDirection.h"
#include "../../gameplay/components/EntityTags.h"
#include "../../gameplay/components/Spell.h"
#include "../../gameplay/systems/SpellManager.h"
#include "../../manager/SpellLibrary.h"

#include "../../utils/VectorMath.h"


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
	SpellData fireballData = {
		1.0f, // damage
		0.5f, // manaCost
		0.2f, // castTime
		0.1f, // cooldowns
		5.0f, // speed
		10.0f, // size
		50.0f, // duration
		15.0f, // radius
		1, // count
		SpellEffect::Burn, // effect
		BehaviorType::Orbit // behaviorType
	};

	SpellManager spellManager(registry);
	SpellLibrary& spellLibrary = SpellLibrary::getInstance();
	spellLibrary.spellDatabase[SpellID::Fireball] = fireballData;
	
	ParticleProperties particleProperties;
	particleProperties.startColor = sf::Color::Red;
	particleProperties.endColor = sf::Color::Yellow;
	particleProperties.sizeEnd = 0.0f;
	particleProperties.sizeStart = 5.0f;
	particleProperties.lifetime = 5.0f;
	particleProperties.velocity = { 0.0f, 0.f };
	particleProperties.velocityVariation = { -2.0f, 2.0f };
	particleProperties.behaviorType = ParticleBehaviorType::Floating;

	for (int i = 0; i < 10; i++)
	{
		particleSystem->emit(particleProperties);
	}
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
	sf::Vector2i mousePosWindow = sf::Mouse::getPosition(windowManager.getWindow());
	sf::Vector2f mousePos = windowManager.getWindow().mapPixelToCoords(mousePosWindow);

	LookingDirection& lookingDirection = registry.get<LookingDirection>(playerEntity);
	lookingDirection = LookingDirection{
		static_cast<float>(mousePos.x - playerPosition.x),
		static_cast<float>(mousePos.y - playerPosition.y) };
	
	normalize(lookingDirection);
}
