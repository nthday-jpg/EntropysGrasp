#include "gameplayInputHandler.h"
#include "commands/playerControl.h"
#include "../components/movementComponents.h"
#include "../components/lookingDirection.h"

using namespace sf;
using namespace std;

void GameplayInputHandler::initCommandFactory()
{
    // Map action strings to command creation functions
    commandFactory["MOVELEFT"] = [this]() { return new MoveLeft(playerEntity); };
    commandFactory["MOVERIGHT"] = [this]() { return new MoveRight(playerEntity); };
    commandFactory["MOVEUP"] = [this]() { return new MoveUp(playerEntity); };
    commandFactory["MOVEDOWN"] = [this]() { return new MoveDown(playerEntity); };
    commandFactory["CAST"] = [this]() { return new CastSpell(playerEntity); };
    commandFactory["DASH"] = [this]() { return new Dash(playerEntity); };
    // Add more commands as needed
}

Command* GameplayInputHandler::createCommand(const std::string& action)
{
    auto it = commandFactory.find(action);
    if (it != commandFactory.end()) {
        return it->second(); // Call the factory function
    }
    return nullptr; // Unknown action
}

void GameplayInputHandler::handleKeyBoard()
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

void GameplayInputHandler::handleMouse()
{
	Command* look = new LookAtMouse(playerEntity);
    if (commandManager)
    {
		commandManager->queueCommand(look);
    }
}

void GameplayInputHandler::handleInput()
{
    Command* resetCmd = new ResetTempComponents(playerEntity);
    if (commandManager) {
        commandManager->queueCommand(resetCmd);
    }

    handleKeyBoard();
    handleMouse();
}

GameplayInputHandler::GameplayInputHandler(entt::entity playerEntity, GameplayCommandManager* commandManager)
    : playerEntity(playerEntity), commandManager(commandManager)
{
    initCommandFactory();
	
    // Initialize keybindings from gameConfig if needed
    // keyBindings = gameConfig->getKeyBindings(); // Assuming gameConfig is accessible
    // For now, we can initialize keyBindings with some default values or leave it empty
    keyBindings = {
		{Keyboard::Scancode::A, "moveLeft"},
		{Keyboard::Scancode::D, "moveRight"},
		{Keyboard::Scancode::W, "moveUp"},
		{Keyboard::Scancode::S, "moveDown"},
	};

}

GameplayInputHandler::~GameplayInputHandler()
{
    for(auto& pair : commandFactory) {
		Command* cmd = pair.second();
		delete cmd; 
	}
}