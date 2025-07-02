#pragma once
#include "../UICommand.h"
#include "../UICommandManager.h"

class Pause : public UICommand
{
	UICommandManager* commandManager;
public: 
	Pause(UICommandManager* commandManager) : commandManager(commandManager) {}
	void execute() override;
};

class Resume : public UICommand
{
	UICommandManager* commandManager;
public: 
	Resume(UICommandManager* commandManager) : commandManager(commandManager) {}
	void execute() override;
};

class Restart : public UICommand
{
	UICommandManager* commandManager;
public: 
	Restart(UICommandManager* commandManager) : commandManager(commandManager) {}
	void execute() override;
};

class Exit : public UICommand
{
	UICommandManager* commandManager;
public:
	Exit(UICommandManager* commandManager) : commandManager(commandManager) {}
	void execute() override;
};
