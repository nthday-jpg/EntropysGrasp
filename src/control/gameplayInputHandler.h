#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <entt/entity/entity.hpp>
#include <map>
#include <functional>
#include <string>
#include "command.h"
#include "gameplayCommandManager.h"

class GameplayInputHandler {
	entt::entity playerEntity;
	GameplayCommandManager* commandManager;
	//GameConfig* gameConfig;

	//remove this later, just for testing
	std::map<sf::Keyboard::Scancode, std::string> keyBindings;

	std::map<std::string, std::function<Command* ()>> commandFactory;
	void initCommandFactory();

	void handleKeyBoard();
	void handleMouse();

	Command* createCommand(const std::string& action);

public:
	GameplayInputHandler(entt::entity playerEntity, GameplayCommandManager* commandManager);
	~GameplayInputHandler();


	void handleInput();
};