#pragma once
#include "command.h"
#include <map>
#include <queue>

class CommandManager {
	std::queue<Command*> commandQueue;
	entt::registry& registry;
public:
	CommandManager(entt::registry registry) : registry(registry) {}
	~CommandManager() = default;

	void executeCommands();

	void queueCommand(Command* command);
};