#pragma once
#include "../command.h"
#include <entt/entt.hpp>

class MoveLeft : public Command
{
	entt::entity playerEntity;
public:
	MoveLeft(entt::entity player) : playerEntity(player) {}
	void execute(entt::registry& registry) override;
};

class MoveRight : public Command
{
	entt::entity playerEntity;
public:
	MoveRight(entt::entity player) : playerEntity(player) {}
	void execute(entt::registry& registry) override;
};

class MoveUp : public Command
{
	entt::entity playerEntity;
public:
	MoveUp(entt::entity player) : playerEntity(player) {}
	void execute(entt::registry& registry) override;
};

class MoveDown : public Command
{
	entt::entity playerEntity;
public:
	MoveDown(entt::entity player) : playerEntity(player) {}
	void execute(entt::registry& registry) override;
};

class CastSpell : public Command
{
	entt::entity playerEntity;
public:
	CastSpell(entt::entity player) : playerEntity(player) {}
	void execute(entt::registry& registry) override;
};

class Dash : public Command
{
	entt::entity playerEntity;
public:
	Dash(entt::entity player) : playerEntity(player) {}
	void execute(entt::registry& registry) override;
};

//this command is used to reset movement direction, look direction
class ResetTempComponents : public Command
{
	entt::entity playerEntity;
public:
	ResetTempComponents(entt::entity player) : playerEntity(player) {}
	void execute(entt::registry& registry) override;
};
