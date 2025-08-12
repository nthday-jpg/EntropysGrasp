#include "GameplayScene.h"
#include "../../gameplay/components/MovementComponents.h"
#include "../../gameplay/components/LookingDirection.h"
#include "../../gameplay/components/EntityTags.h"
#include "../../gameplay/components/StatComponent.h"
#include "../../control/commands/GameControl.h"
#include "../../gameplay/components/Hitbox.h"
#include "../../ui/Button.h"
#include "../../manager/FontManager.h"
#include "../../manager/TextureManager.h"
#include "../../manager/AnimationManager.h"

GameplayScene::GameplayScene(sf::RenderWindow& window, entt::dispatcher* dispatcher) 
	: Scene(window), dispatcher(dispatcher),
    collisionSystem(registry), 
    movementPipeline(registry),
    combatSystem(registry),
    spellManager(registry),
	camera(&registry),
	enemyManager(registry, camera, gameClock),
	physicsSystem(registry),
    renderSystem(registry), 
	particleSystem(registry),
	animationSystem(registry),
	rewardSystem(registry)
{
    
    MapManager::getInstance().loadMap();
	TextureManager::getInstance().loadFromAssetFile();
	gameplayCommandManager = new GameplayCommandManager(registry);
	entt::entity playerEntity = createPlayer();

    inputHandler = new GameplayInputHandler(playerEntity, gameplayCommandManager);
	camera.followEntity(playerEntity);
    window.setView(camera.getView());

	pausedUI = new UIManager();
    pausedUI->load();

    inputHandler->particleSystem = &particleSystem;
	inputHandler->spellManager = &spellManager;

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
    // Store dispatcher in registry context for all systems to access
    registry.ctx().emplace<entt::dispatcher*>(dispatcher);
    collisionSystem.sinkEvents();
	combatSystem.sinkEvents();
	spellManager.sinkEvents();
	enemyManager.sinkEvents();
    animationSystem.sinkEvents();
    rewardSystem.sinkEvents();
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
    
    // Process all queued events (including animation events)
    
    // Update UI
    // Here you would typically run your game systems like:
    // - Movement system
    // - Collision system
    // - Rendering system
    // - etc.
    spellManager.update(deltaTime);
    collisionSystem.update(deltaTime);
    movementPipeline.update(deltaTime);
    physicsSystem.updateVelocity(deltaTime);
    enemyManager.update(deltaTime);
	particleSystem.update(deltaTime);
	animationSystem.update(deltaTime);
    if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
        (*dispatcher)->update();
    }

    // Update camera
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
    registry.emplace<Speed>(player, 200.0f);
    registry.emplace<Health>(player, 10000.0f, 10000.0f);
    registry.emplace<Attack>(player, 100.0f);
    registry.emplace<Hitbox>(player, Hitbox{25.0f, 20.0f, 0.0f, 0.0f});
    registry.emplace<MovementDirection>(player, 0.0f, 0.0f);
    registry.emplace<LookingDirection>(player, 0.0f, 0.0f);
    registry.emplace<Mana>(player, 1000.0f);
    registry.emplace<RepelResistance>(player, 0.5f);

    sf::Texture* mageTexture = TextureManager::getInstance().getTexture("Mage");

    AnimationComponent animComp;
    animComp.name = "Mage";
    animComp.currentState = AnimationState::Idle;
    animComp.currentDirection = Direction::Down;
    animComp.currentFrame = { 0, 0 };
    animComp.timer = 0.0f;
    registry.emplace<AnimationComponent>(player, animComp);

    // sprite gắn vào để render
    sf::IntRect textureRect({ 0, 0 }, { 32, 48}); // Assuming each frame is 50x50 pixels
    sf::Sprite sprite(*mageTexture);
	sprite.setTextureRect(textureRect);
	sprite.setOrigin({ 16.f, 24.f }); // Set origin to center of the sprite
    sprite.setPosition({ 0.f, 0.f });

    registry.emplace<sf::Sprite>(player, sprite);	
    return player;
}