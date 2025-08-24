#pragma once
#include "../GameCommand.h"
#include "../GameCommandManager.h"
#include "../../manager/SceneManager.h"
#include <string>

// Control commands for game control actions
// The bindings of these commands are defined in Button.h

class Pause : public GameCommand
{
	GameCommandManager* commandManager;
public: 
	Pause() : commandManager(&GameCommandManager::getInstance()) {}
	void execute() override;
};

class Restart : public GameCommand
{
	GameCommandManager* commandManager;
public: 
	Restart() : commandManager(&GameCommandManager::getInstance()) {}
	void execute() override;
};

class Resume : public GameCommand
{
	GameCommandManager* commandManager;
public:
	Resume() : commandManager(&GameCommandManager::getInstance()) {}
	void execute() override;
};

class Exit : public GameCommand
{
	GameCommandManager* commandManager;
public:
	Exit() : commandManager(&GameCommandManager::getInstance()) {}
	void execute() override;
};


class ChangeScene : public GameCommand
{
    SceneManager* sceneManager;
    std::string targetScene;
public:
    ChangeScene(std::string targetScene) :
        sceneManager(&SceneManager::getInstance()),
        targetScene(targetScene) { }
	void execute() override;
};

class Shutdown : public GameCommand
{
	GameCommandManager* commandManager;
public:
	Shutdown() : commandManager(&GameCommandManager::getInstance()) {}
	void execute() override;
};