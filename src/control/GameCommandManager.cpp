#include "GameCommandManager.h"
#include "GameCommand.h"

void GameCommandManager::queueCommand(GameCommand* cmd) {
	if (cmd) 
	{
		commandQueue.push(cmd);
	}
}

void GameCommandManager::executeCommands() {
	while (!commandQueue.empty()) {
		GameCommand* cmd = commandQueue.front();
		commandQueue.pop();
		if (cmd) {
			cmd->execute();
			delete cmd; 
		}
	}
}

GameCommandManager::~GameCommandManager() {
	while (!commandQueue.empty()) {
		GameCommand* cmd = commandQueue.front();
		commandQueue.pop();
		delete cmd; 
	}
}