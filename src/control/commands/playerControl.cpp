#include "playerControl.h"
#include "../../components/movementComponents.h"
#include "../../components/lookingDirection.h"
#include "../../components/EntityTags.h"

#include <iostream>

const MovementDirection down = MovementDirection(0, 1);
const MovementDirection up = MovementDirection(0, -1);
const MovementDirection left = MovementDirection(-1, 0);
const MovementDirection right = MovementDirection(1, 0);

void MoveDown::execute(entt::registry& registry)
{

}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += down;
}

void MoveUp::execute(entt::registry& registry)
{

}
	MovementDirection& direction = registry.get<MovementDirection>(playerEntity);
	direction += up;
}

void MoveLeft::execute(entt::registry& registry)
{

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
		registry.emplace<LookingDirection>(playerEntity, 0.0f);
	}
	else
	{
		registry.get<LookingDirection>(playerEntity) = LookingDirection(0.0f, 0.0f);
	}

}