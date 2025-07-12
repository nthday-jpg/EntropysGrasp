#include "GameplayScene.h"
#include "../../gameplay/components/MovementComponents.h"
#include "../../gameplay/components/LookingDirection.h"
#include "../../gameplay/components/EntityTags.h"
#include "../../gameplay/components/StatComponent.h"

GameplayScene::GameplayScene(sf::RenderWindow& window) : Scene(window) {
    inputHandler = nullptr;
	gameplayCommandManager = nullptr;

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

    if (isPaused) {
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

void GameplayScene::pause()
{
	isPaused = true;
}

void GameplayScene::resume()
{
    isPaused = false;
}

void GameplayScene::restart()
{
    // Logic to restart the game, e.g., reset player position, score, etc.
    // This could also involve reloading the scene or resetting components.
}

void GameplayScene::exit()
{
    // Logic to exit the game, e.g., navigate to main menu or quit application.
}