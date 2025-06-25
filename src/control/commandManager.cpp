#include "commandManager.h"
#include <entt/entity/registry.hpp>

void CommandManager::queueCommand(Command* command) {
	commandQueue.push(command);
}

void CommandManager::executeCommands() {
	while (!commandQueue.empty()) {
		Command* command = commandQueue.front();
		commandQueue.pop();
		command->execute(registry);
		delete command;
	}
}