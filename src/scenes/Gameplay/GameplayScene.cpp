#include "GameplayScene.h"
#include "../../gameplay/components/MovementComponents.h"
#include "../../gameplay/components/LookingDirection.h"
#include "../../gameplay/components/EntityTags.h"
#include "../../gameplay/components/StatComponent.h"
#include "../../control/commands/GameControl.h"
#include "../../gameplay/components/Hitbox.h"
#include "../../ui/Button.h"
#include "../../manager/FontManager.h"

GameplayScene::GameplayScene(sf::RenderWindow& window) 
    : Scene(window) , 
    collisionSystem(registry),
    movementPipeline(registry),
    combatSystem(registry),
    spellManager(registry),
	enemyManager(registry, camera.getView(), gameClock),
	physicsSystem(registry),
    renderSystem(registry), 
	particleSystem(registry)
{
	gameplayCommandManager = new GameplayCommandManager(registry);
    inputHandler = new GameplayInputHandler(createPlayer(), gameplayCommandManager);

	pausedUI = new UIManager();
    pausedUI->load();

    inputHandler->particleSystem = &particleSystem;

	uiManager = new UIManager();
    uiManager->addElement(
        new Button(
            "EXIT",
            "Exit",
            FontManager::getInstance().getFont("default"),
            { 100, 100 },
            30.0f
        )
	);
}

GameplayScene::~GameplayScene() {
    if (inputHandler) {
        delete inputHandler;
    }
    if (gameplayCommandManager) {
        delete gameplayCommandManager;
    }
}

void GameplayScene::load() 
{
    
	isLoaded = true;    
}

void GameplayScene::unload() {
    // Unload resources, reset state, etc.
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
		pausedUI->draw(window);
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
    
    // Here you would typically run your game systems like:
    // - Movement system
    // - Collision system
    // - Rendering system
    // - etc.
    collisionSystem.detectCollisions();
    movementPipeline.update(deltaTime);
    //combatSystem.update(deltaTime);
    spellManager.update(deltaTime);
    enemyManager.update(deltaTime);
    physicsSystem.updateVelocity(deltaTime);
	particleSystem.update(deltaTime);
    // Update camera
    Position playerPos = registry.get<Position>(registry.view<PlayerTag>().front());
    camera.setPosition(playerPos);
    camera.update(deltaTime);
    window.setView(camera.getView());

    if (uiManager) {
        uiManager->syncUIWithViewport();
    }
    // Render the scene
    this->render();

}

void GameplayScene::render() {
    // Render game objects
    // This is where you would typically render your sprites, entities, etc.
    renderSystem.render();

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
	this->unload();
	this->load();
}

void GameplayScene::exit()
{
	UICommandManager::getInstance().queueCommand(new ChangeScene("MainMenu"));
}

entt::entity GameplayScene::createPlayer() {
    auto player = registry.create();
    registry.emplace<PlayerTag>(player);
    registry.emplace<Position>(player, 0.0f, 0.0f);
    registry.emplace<Speed>(player, 800.0f);
    registry.emplace<Hitbox>(player, 50.0f, 50.0f, 0.0f, 0.0f);
    registry.emplace<MovementDirection>(player, 0.0f, 0.0f);
    registry.emplace<LookingDirection>(player, 0.0f, 0.0f);
    registry.emplace<Mana>(player, 100.0f);
    registry.emplace<RepelResistance>(player, 0.5f);

	sf::Texture* playerTexture = new sf::Texture("assets/texture/test.png");
	sf::Sprite* playerSprite = new sf::Sprite(*playerTexture);
    playerSprite->setPosition({ 0.0f, 0.0f }); // Set initial position to match entity position
    playerSprite->setTextureRect(sf::IntRect({ 0, 0 }, { 50, 50 })); // Set texture rect to match hitbox size

	registry.emplace<sf::Sprite>(player, *playerSprite);

    entt::entity blockS = registry.create();
	sf::Sprite* block = new sf::Sprite(*playerTexture);
	block->setPosition({ 10.f, 10.0f });
	block->setTextureRect(sf::IntRect({ 0, 0 }, { 50, 50 }));
	registry.emplace<Position>(blockS, 10.0f, 10.0f);
	registry.emplace<sf::Sprite>(blockS, *block);
    
    return player;
}