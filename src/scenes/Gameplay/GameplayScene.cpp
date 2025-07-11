#include "GameplayScene.h"
#include "../../gameplay/components/movementComponents.h"
#include "../../gameplay/components/lookingDirection.h"
#include "../../gameplay/components/EntityTags.h"
#include "../../gameplay/components/statComponent.h"

GameplayScene::GameplayScene(sf::RenderWindow& window) : Scene(window), playerEntity(entt::null) {
    gameplayCommandManager = new GameplayCommandManager(registry);
    inputHandler = nullptr;
}

GameplayScene::~GameplayScene() {
    if (inputHandler) {
        delete inputHandler;
    }
    if (gameplayCommandManager) {
        delete gameplayCommandManager;
    }
}

void GameplayScene::load() {
    if (!isLoaded) {
        // Create the player entity and components
        createPlayer();
        
        // Initialize the input handler after creating the player
        if (inputHandler) {
            delete inputHandler;
        }
        inputHandler = new GameplayInputHandler(playerEntity, gameplayCommandManager);
        
        isLoaded = true;
    }
}

void GameplayScene::createPlayer() {
    // Create player entity with necessary components
    playerEntity = registry.create();
    
    // Add player tag
    registry.emplace<PlayerTag>(playerEntity);
    
    // Add position component
    registry.emplace<Position>(playerEntity, 400.0f, 300.0f); // Center of an 800x600 window
    
    // Add movement direction component (starts at zero)
    registry.emplace<MovementDirection>(playerEntity, 0.0f, 0.0f);
    
    // Add looking direction component
    registry.emplace<LookingDirection>(playerEntity, 0.0f, 0.0f);
    
    // Add health component
    registry.emplace<Health>(playerEntity, 100.0f, 100.0f); // max health, current health
}

bool GameplayScene::handleEvent(const std::optional<sf::Event>& event) {
    bool handled = false;
    
    // First let the UI handle the event
    if (uiManager) {
        handled = uiManager->handleEvent(event);
    }
    
    // If UI didn't handle it, let the input handler handle discrete events
    if (!handled && inputHandler) {
        handled = inputHandler->handleEvent(event);
    }
    
    return handled;
}

void GameplayScene::update(float deltaTime) {
    if (!isLoaded) {
        load();
        return;
    }
    
    // Handle continuous input (movement, looking, etc.)
    if (inputHandler) {
        inputHandler->handleInput();
    }
    
    // Execute all queued gameplay commands
    if (gameplayCommandManager) {
        gameplayCommandManager->executeCommands();
    }
    
    // Update UI
    if (uiManager) {
        uiManager->syncUIWithViewport();
    }
    
    // Here you would typically run your game systems like:
    // - Movement system
    // - Collision system
    // - Rendering system
    // - etc.
}

void GameplayScene::render() {
    // Render game objects
    // This is where you would typically render your sprites, entities, etc.
    
    // Render UI on top
    if (uiManager) {
        uiManager->draw(window);
    }
}