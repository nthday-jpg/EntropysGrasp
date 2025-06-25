#include "inputHandler.h"
#include "commands/playerControl.h"
#include "../components/movementComponents.h"

using namespace sf;
using namespace std;

void InputHandler::initCommandFactory()
{
    // Map action strings to command creation functions
    commandFactory["moveLeft"] = [this]() { return new MoveLeft(playerEntity); };
    commandFactory["moveRight"] = [this]() { return new MoveRight(playerEntity); };
    commandFactory["moveUp"] = [this]() { return new MoveUp(playerEntity); };
    commandFactory["moveDown"] = [this]() { return new MoveDown(playerEntity); };
    commandFactory["castSpell"] = [this]() { return new CastSpell(playerEntity); };
    commandFactory["dash"] = [this]() { return new Dash(playerEntity); };
    // Add more commands as needed
}

Command* InputHandler::createCommand(const std::string& action)
{
    auto it = commandFactory.find(action);
    if (it != commandFactory.end()) {
        return it->second(); // Call the factory function
    }
    return nullptr; // Unknown action
}

void InputHandler::handleKeyBoard()
{
    // Use gameConfig's keybindings directly
    //const auto& keyBindings = gameConfig->getKeyBindings();

    for (const auto& binding : keyBindings) {
        if (sf::Keyboard::isKeyPressed(binding.first)) {
            Command* cmd = createCommand(binding.second);
            if (cmd && commandManager) {
                commandManager->queueCommand(cmd);
            }
        }
    }
}

void InputHandler::handleMouse()
{

}

void InputHandler::handleInput()
{
    Command* resetCmd = new ResetTempComponents(playerEntity);
    if (commandManager) {
        commandManager->queueCommand(resetCmd);
    }

    handleKeyBoard();
    handleMouse();
}