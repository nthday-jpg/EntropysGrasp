#pragma once
#include "../command.h"
#include <entt/entt.hpp>
#include <string>
#include "../../gameplay/systems/ParticleSystem.h"
#include "../../gameplay/systems/SpellManager.h"


// The bindings of string with these commands are defined in gameplayInputHandler.h
// and bindings of string with the keyboard keys are defined in gameConfig.h
// The movement direction will be normalized to length 1 in playerVelocitySystem

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
	ParticleSystem* particleSystem;
	SpellManager* spellManager;
public:
	CastSpell(entt::entity player, ParticleSystem* particleSystem, SpellManager* spellManager) 
		: playerEntity(player), particleSystem(particleSystem), spellManager(spellManager) {}
	void execute(entt::registry& registry) override;
};

class Dash : public Command
{
	entt::entity playerEntity;
public:
	Dash(entt::entity player) : playerEntity(player) {}
	void execute(entt::registry& registry) override;
};

class LookAtMouse: public Command
{
	entt::entity playerEntity;
public:
	LookAtMouse(entt::entity player) : playerEntity(player) {}
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