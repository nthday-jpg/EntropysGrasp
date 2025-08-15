#pragma once
#include "Command.h"
#include <map>
#include <queue>

class GameplayCommandManager {
	std::queue<Command*> commandQueue;
	entt::registry& registry;

public:
	GameplayCommandManager(entt::registry& registry) : registry(registry) {}
	~GameplayCommandManager();

	void executeCommands();

	void queueCommand(Command* command);

	void clear();
};