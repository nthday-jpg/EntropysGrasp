#pragma once
#include <entt/entt.hpp>

// update direction only if the command is moved

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(entt::registry& registry) = 0;
};

class MoveLeftCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// Implementation for moving left
	}
};

class MoveRightCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// Implementation for moving right
	}
};	

class MoveUpCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// Implementation for moving up
	}
};

class MoveDownCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// Implementation for moving down
	}
};

class MoveUpLeftCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// Implementation for moving up-left
	}
};

class MoveUpRightCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// Implementation for moving up-right
	}
};

class MoveDownLeftCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// Implementation for moving down-left
	}
};

class MoveDownRightCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// Implementation for moving down-right
	}
};

class ShootCommand : public Command
{
	public:
	void execute(entt::registry& registry) override
	{
		// call the spell system
	}
};