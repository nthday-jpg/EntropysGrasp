#pragma once
#include <entt/entity/registry.hpp>

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(entt::registry& registry) = 0;
};