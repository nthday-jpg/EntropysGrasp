#include "GameplayInputHandler.h"
#include "commands/playerControl.h"
#include "../gameplay/components/MovementComponents.h"
#include "../gameplay/components/LookingDirection.h"

using namespace sf;
using namespace std;

void GameplayInputHandler::initCommandFactory()
{
    // Map action strings to command creation functions
    commandFactory["MOVELEFT"] = [this]() { return new MoveLeft(playerEntity); };
    commandFactory["MOVERIGHT"] = [this]() { return new MoveRight(playerEntity); };
    commandFactory["MOVEUP"] = [this]() { return new MoveUp(playerEntity); };
    commandFactory["MOVEDOWN"] = [this]() { return new MoveDown(playerEntity); };
    commandFactory["CAST"] = [this]() { return new CastSpell(playerEntity, particleSystem, spellManager); };
    commandFactory["DASH"] = [this]() { return new Dash(playerEntity); };
    // Add more commands as needed
}

void GameplayInputHandler::initActionTypes()
{
    // Define which actions are continuous (held down) vs discrete (single press)
    continuousActions = {
        "MOVELEFT", "MOVERIGHT", "MOVEUP", "MOVEDOWN"
    };
    
    discreteActions = {
        "CAST", "DASH", "PAUSE"
    };
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
    // Handle continuous input only (keys held down)
    for (const auto& binding : *keyBindings) {
        const std::string& action = binding.second;
        
        if (continuousActions.find(action) == continuousActions.end()) 
        {
			continue; // Skip discrete actions
        }
        if (sf::Keyboard::isKeyPressed(binding.first)) {
            Command* cmd = createCommand(action);
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

bool GameplayInputHandler::handleEvent(const std::optional<sf::Event>& event)
{
    if (!event.has_value()) {
        return false;
    }

    bool handled = false;

    // Handle discrete key press events
    if (const auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        // Find the action associated with this key
        for (const auto& binding : *keyBindings) {
            if (binding.first == keyPressed->scancode) {
                const std::string& action = binding.second;
                
                // Only process discrete actions in this method
                if (discreteActions.find(action) != discreteActions.end()) {
                    Command* cmd = createCommand(action);
                    if (cmd && commandManager) {
                        commandManager->queueCommand(cmd);
                        handled = true;
                    }
                }
                break;
            }
        }
    }

    // Handle discrete mouse button events if needed
    else if (const auto mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
		handled = true;
    }

    return handled;
}

GameplayInputHandler::GameplayInputHandler(
    entt::entity playerEntity, GameplayCommandManager* commandManager
) : playerEntity(playerEntity),
    commandManager(commandManager),
    gameConfig(&GameConfig::getInstance()),
    keyBindings(gameConfig->getKeyBindings())
{
    initCommandFactory();
    initActionTypes();
}

GameplayInputHandler::~GameplayInputHandler()
{
    for (auto& pair : commandFactory) {
        Command* cmd = pair.second();
        delete cmd;
    }
}