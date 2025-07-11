#include "GameplayCommandManager.h"
#include <entt/entity/registry.hpp>

void GameplayCommandManager::queueCommand(Command* command) {
	if (command)
	{
		commandQueue.push(command);
	}
}

GameplayCommandManager::~GameplayCommandManager()
{
	while (!commandQueue.empty()) {
		Command* command = commandQueue.front();
		commandQueue.pop();
		delete command;
	}
}

void GameplayCommandManager::executeCommands() {
	while (!commandQueue.empty()) {
		Command* command = commandQueue.front();
		commandQueue.pop();
		command->execute(registry);
		delete command;
	}
}