#pragma once
#include <entt/entt.hpp>

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(entt::registry& registry) = 0;
};