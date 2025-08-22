#pragma once
#include "../UICommand.h"
#include "../UICommandManager.h"
#include "../../manager/SceneManager.h"
#include <string>

// Control commands for game control actions
// The bindings of these commands are defined in Button.h

class Pause : public UICommand
{
	UICommandManager* commandManager;
public: 
	Pause() : commandManager(&UICommandManager::getInstance()) {}
	void execute() override;
};

class Restart : public UICommand
{
	UICommandManager* commandManager;
public: 
	Restart() : commandManager(&UICommandManager::getInstance()) {}
	void execute() override;
};


class Exit : public UICommand
{
	UICommandManager* commandManager;
public:
	Exit() : commandManager(&UICommandManager::getInstance()) {}
	void execute() override;
};


class ChangeScene : public UICommand
{
    SceneManager* sceneManager;
    std::string targetScene;
public:
    ChangeScene(std::string targetScene) :
        sceneManager(&SceneManager::getInstance()),
        targetScene(targetScene) { }
	void execute() override;
};