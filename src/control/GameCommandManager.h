#pragma once
#include <queue>
#include "GameCommand.h"

class GameCommandManager
{
    std::queue<GameCommand*> commandQueue;

    GameCommandManager() = default;

public:
    GameCommandManager(const GameCommandManager&) = delete;
    GameCommandManager& operator=(const GameCommandManager&) = delete;
    GameCommandManager(GameCommandManager&&) = delete;
    GameCommandManager& operator=(GameCommandManager&&) = delete;

    ~GameCommandManager();

    static GameCommandManager& getInstance() {
        static GameCommandManager instance;
        return instance;
    }

    void queueCommand(GameCommand* command);
    void executeCommands();
};