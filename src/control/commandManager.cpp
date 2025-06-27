#include "commandManager.h"
#include <entt/entity/registry.hpp>

void CommandManager::queueCommand(Command* command) {
	commandQueue.push(command);
}

CommandManager::~CommandManager()
{
	while (!commandQueue.empty()) {
		Command* command = commandQueue.front();
		commandQueue.pop();
		delete command; // Clean up remaining commands
	}
}

void CommandManager::executeCommands() {
	while (!commandQueue.empty()) {
		Command* command = commandQueue.front();
		commandQueue.pop();
		command->execute(registry);
		delete command;
	}
}