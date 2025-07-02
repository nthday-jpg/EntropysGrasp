#pragma once
#include <queue>
#include "UICommand.h"

class UICommandManager
{
    std::queue<UICommand*> commandQueue;

    UICommandManager() = default;

public:
    UICommandManager(const UICommandManager&) = delete;
    UICommandManager& operator=(const UICommandManager&) = delete;
    UICommandManager(UICommandManager&&) = delete;
    UICommandManager& operator=(UICommandManager&&) = delete;

    ~UICommandManager();

    static UICommandManager& getInstance() {
        static UICommandManager instance;
        return instance;
    }

    void queueCommand(UICommand* command);
    void executeCommands();
};