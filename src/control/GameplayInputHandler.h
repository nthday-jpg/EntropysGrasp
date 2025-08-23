#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <entt/entity/entity.hpp>
#include <map>
#include <functional>
#include <string>
#include <set>
#include <optional>
#include "Command.h"
#include "gameplayCommandManager.h"
#include "../GameConfig.h"
#include "../gameplay/systems/ParticleSystem.h"
#include "../gameplay/systems/SpellManager.h"

class GameplayInputHandler {
	entt::entity playerEntity;
	GameplayCommandManager* commandManager;
	GameConfig* gameConfig;
	const std::map<sf::Keyboard::Scancode, std::string>* keyBindings;
	
	// Define which actions are continuous (held down) vs discrete (single press)
	std::set<std::string> continuousActions;
	std::set<std::string> discreteActions;
	std::map<std::string, std::function<Command* ()>> commandFactory;
	
	// Add pause state awareness
	bool* isPausedPtr; // Pointer to GameplayScene's isPaused
	
	void initCommandFactory();
	void initActionTypes();
	void handleKeyBoard();
	void handleMouse();
	Command* createCommand(const std::string& action);

public:
	SpellManager* spellManager;
	
	GameplayInputHandler(entt::entity playerEntity, GameplayCommandManager* commandManager);
	~GameplayInputHandler();
	
	// Add method to set pause state pointer
	void setPauseStatePtr(bool* pausedPtr);
	
	// Handle continuous input (called every frame)
	void handleInput();
	
	// Handle discrete events (called when events occur)
	bool handleEvent(const std::optional<sf::Event>& event);
};