#include "UICommandManager.h"
#include "UICommand.h"

void UICommandManager::queueCommand(UICommand* cmd) {
	if (cmd) 
	{
		commandQueue.push(cmd);
	}
}

void UICommandManager::executeCommands() {
	while (!commandQueue.empty()) {
		UICommand* cmd = commandQueue.front();
		commandQueue.pop();
		if (cmd) {
			cmd->execute();
			delete cmd; 
		}
	}
}

UICommandManager::~UICommandManager() {
	while (!commandQueue.empty()) {
		UICommand* cmd = commandQueue.front();
		commandQueue.pop();
		delete cmd; 
	}
}