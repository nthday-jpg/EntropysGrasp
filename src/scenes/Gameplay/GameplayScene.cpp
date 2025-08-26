#include "GameplayScene.h"
#include "../../gameplay/components/MovementComponents.h"
#include "../../gameplay/components/LookingDirection.h"
#include "../../gameplay/components/EntityTags.h"
#include "../../gameplay/components/StatComponent.h"
#include "../../control/commands/GameControl.h"
#include "../../gameplay/components/Hitbox.h"
#include "../../ui/Button.h"
#include "../../ui/Text.h"
#include "../../ui/UIOrigin.h"  
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
	rewardSystem(registry),
	stateSystem(registry)
{
    gameplayCommandManager = new GameplayCommandManager(registry);
    // Store dispatcher in registry context for all systems to access
    registry.ctx().emplace<entt::dispatcher*>(dispatcher);
    collisionSystem.sinkEvents();
	combatSystem.sinkEvents();
	spellManager.sinkEvents();
	enemyManager.sinkEvents();
    rewardSystem.sinkEvents();
	stateSystem.sinkEvents();
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
    MapManager::getInstance().loadMap();
    TextureManager::getInstance().loadFromAssetFile();
    playerEntity = createPlayer(); // Store as member variable

    inputHandler = new GameplayInputHandler(playerEntity, gameplayCommandManager);
    
    // Connect the pause state to the input handler
    inputHandler->setPauseStatePtr(&isPaused);
    
    camera.followEntity(playerEntity);

    inputHandler->spellManager = &spellManager;

    uiManager = new UIManager();
	uiManager->loadFile(uiFilePath);

    // Add dynamic text elements for real-time state - Left side HUD
    Text* rewardText = new Text(
        *FontManager::getInstance().getFont("default"), 
        "Rewards: 0", 
        {20.0f, 10.0f}, 
        20
    );
    rewardText->setOrigin(UIOrigin::TopLeft);
    
	uiManager->addDynamicText("rewardText", 
		rewardText,
		[this]() -> std::string {
			int currentReward = rewardSystem.getCurrentReward();
			return "Rewards: " + std::to_string(currentReward);
		}
	);
	
    Text* timeText = new Text(
        *FontManager::getInstance().getFont("default"), 
        "Time: 00:00", 
        {20.0f, 40.0f}, 
        20
    );
    timeText->setOrigin(UIOrigin::TopLeft);
    
	uiManager->addDynamicText("timeText",
		timeText,
		[this]() -> std::string {
			int totalSeconds = static_cast<int>(gameClock.getElapsedTime().asSeconds());
			int minutes = totalSeconds / 60;
			int seconds = totalSeconds % 60;
			return "Time: " + std::to_string(minutes) + ":" + 
				   (seconds < 10 ? "0" : "") + std::to_string(seconds);
		}
	);

    // Add HP display - Top right corner
    Text* hpText = new Text(
        *FontManager::getInstance().getFont("default"), 
        "HP: 10000/10000", 
        {-20.0f, 10.0f}, 
        22
    );
    hpText->setOrigin(UIOrigin::TopRight);
    
    uiManager->addDynamicText("hpText",
        hpText,
        [this]() -> std::string {
            // Get player's health component
            if (registry.valid(playerEntity) && registry.all_of<Health>(playerEntity)) {
                const auto& health = registry.get<Health>(playerEntity);
                return "HP: " + std::to_string(static_cast<int>(health.current)) + 
                       "/" + std::to_string(static_cast<int>(health.max));
            }
            return "HP: 0/0";
        }
    );

    // Add Mana display - Top right corner below HP
    Text* manaText = new Text(
        *FontManager::getInstance().getFont("default"), 
        "Mana: 1000", 
        {-20.0f, 40.0f}, 
        22
    );
    manaText->setOrigin(UIOrigin::TopRight);
    
    uiManager->addDynamicText("manaText",
        manaText,
        [this]() -> std::string {
            // Get player's mana component
            if (registry.valid(playerEntity) && registry.all_of<Mana>(playerEntity)) {
                const auto& mana = registry.get<Mana>(playerEntity);
                return "Mana: " + std::to_string(static_cast<int>(mana.value));
            }
            return "Mana: 0";
        }
    );

	// Add spell cooldown displays for each usable spell - Left side below time
	addSpellCooldownDisplays();

    window.setView(camera.getView());
}

void GameplayScene::addSpellCooldownDisplays()
{	
		// Create dynamic text for each spell cooldown
		std::string textId = "spellCooldown";
		
        Text* spellText = new Text(
            *FontManager::getInstance().getFont("default"), 
             "Spell : Ready", 
            {20.0f, 70.0f }, // Stack vertically with 25px spacing
            18 // Slightly smaller text
        );
        spellText->setOrigin(UIOrigin::TopLeft); // Use UIOrigin instead of TextOrigin
        
		uiManager->addDynamicText(textId, 
			spellText,
			[this]() -> std::string {
				// Check if spell is on cooldown
		        SpellID spellID = spellManager.currentSpell();
		        const std::string& spellName = spellIDToString(spellID);
				auto cooldownIt = spellManager.cooldowns.find(spellID);
				if (cooldownIt != spellManager.cooldowns.end() && cooldownIt->second > 0.0f) 
				{
					// Format cooldown time to 1 decimal place
					float cooldownTime = cooldownIt->second;
					std::ostringstream oss;
					oss << std::fixed << std::setprecision(1) << cooldownTime;
					return spellName + ": " + oss.str() + "s";
				}
				else 
				{
					return spellName + ": Ready";
				}
			}
		);
}

void GameplayScene::unload() 
{
    // Unload resources, reset state, etc.
	WindowManager::getInstance().reset();
    
    if (uiManager) 
    {
        delete uiManager;
        uiManager = nullptr;
    }
    
    if (inputHandler) 
    {
        delete inputHandler;
        inputHandler = nullptr;
    }
    
    if (gameplayCommandManager)
    {
		gameplayCommandManager->clear();
    }

    registry.clear();
    camera.stopFollowing();
	gameClock.restart();
    
	isLoaded = false;
    isPaused = false;
}

bool GameplayScene::handleEvent(const std::optional<sf::Event>& event) {
    bool handled = false;
    
    // First let the UI handle the event (pause menu gets priority)
    if (uiManager) {
        handled = uiManager->handleEvent(event);
    }
    
    // Let the input handler handle events (it now knows about pause state)
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
        // Update UI even when paused (for pause menu interaction)
        if (uiManager) 
        {
            uiManager->syncUIWithViewport();
            // Don't update dynamic texts when paused to freeze the time display
        }
        // Still render when paused
        this->render();
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
	stateSystem.update(deltaTime);
	animationSystem.update(deltaTime);
    if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>()) {
        (*dispatcher)->update();
    }

    // Update camera
    camera.update(deltaTime);
    window.setView(camera.getView());
	
    if (uiManager) 
    {
        uiManager->syncUIWithViewport();
        uiManager->updateDynamicTexts(); // Update dynamic texts every frame
    }
    // Render the scene
    this->render();
}

void GameplayScene::render() {
    // Render game objects
    // This is where you would typically render your sprites, entities, etc.
    renderSystem.render();

    // Render UI on top (both HUD and pause menu)
    if (uiManager) {
        uiManager->draw(window);
    }
}

void GameplayScene::pause()
{
	isPaused = !isPaused;
	
	// Control the game clock based on pause state
	if (isPaused) {
		gameClock.stop();  // Stop the clock when paused
		showPauseMenu();   // Show pause menu
	} else {
		gameClock.start(); // Resume the clock when unpaused
		hidePauseMenu();   // Hide pause menu
	}
}

void GameplayScene::showPauseMenu() {
	if (uiManager) {
		// Hide all HUD elements
		hideHUDElements();
		
		// Show pause panel
		UIElement* pausePanel = uiManager->getElementByID("pausePanel");
		if (pausePanel) {
			pausePanel->setVisible(true);
		}
	}
}

void GameplayScene::hidePauseMenu() {
	if (uiManager) {
		// Hide pause panel
		UIElement* pausePanel = uiManager->getElementByID("pausePanel");
		if (pausePanel) {
			pausePanel->setVisible(false);
		}
		
		// Show all HUD elements
		showHUDElements();
	}
}

void GameplayScene::hideHUDElements() {
	if (uiManager) {
		// Hide dynamic text elements
		auto rewardText = uiManager->dynamicTexts.find("rewardText");
		if (rewardText != uiManager->dynamicTexts.end()) {
			rewardText->second->setVisible(false);
		}
		
		auto timeText = uiManager->dynamicTexts.find("timeText");
		if (timeText != uiManager->dynamicTexts.end()) {
			timeText->second->setVisible(false);
		}
		
		// Hide HP and Mana displays
		auto hpText = uiManager->dynamicTexts.find("hpText");
		if (hpText != uiManager->dynamicTexts.end()) {
			hpText->second->setVisible(false);
		}
		
		auto manaText = uiManager->dynamicTexts.find("manaText");
		if (manaText != uiManager->dynamicTexts.end()) {
			manaText->second->setVisible(false);
		}
		
		// Hide spell cooldown texts
		for (int i = 1; i <= 4; ++i) {
			std::string textId = "spell" + std::to_string(i) + "Cooldown";
			auto spellText = uiManager->dynamicTexts.find(textId);
			if (spellText != uiManager->dynamicTexts.end()) {
				spellText->second->setVisible(false);
			}
		}
	}
}

void GameplayScene::showHUDElements() {
	if (uiManager) {
		// Show dynamic text elements
		auto rewardText = uiManager->dynamicTexts.find("rewardText");
		if (rewardText != uiManager->dynamicTexts.end()) {
			rewardText->second->setVisible(true);
		}
		
		auto timeText = uiManager->dynamicTexts.find("timeText");
		if (timeText != uiManager->dynamicTexts.end()) {
			timeText->second->setVisible(true);
		}
		
		// Show HP and Mana displays
		auto hpText = uiManager->dynamicTexts.find("hpText");
		if (hpText != uiManager->dynamicTexts.end()) {
			hpText->second->setVisible(true);
		}
		
		auto manaText = uiManager->dynamicTexts.find("manaText");
		if (manaText != uiManager->dynamicTexts.end()) {
			manaText->second->setVisible(true);
		}
		
		// Show spell cooldown texts
		for (int i = 1; i <= 4; ++i) {
			std::string textId = "spell" + std::to_string(i) + "Cooldown";
			auto spellText = uiManager->dynamicTexts.find(textId);
			if (spellText != uiManager->dynamicTexts.end()) {
				spellText->second->setVisible(true);
			}
		}
	}
}

void GameplayScene::restart()
{
	this->unload();
	this->load();
}

void GameplayScene::exit()
{
	GameCommandManager::getInstance().queueCommand(new ChangeScene("MainMenu"));
}

entt::entity GameplayScene::createPlayer() {
    auto player = registry.create();
    registry.emplace<PlayerTag>(player);
    registry.emplace<Position>(player, 400.0f, 300.0f);
    registry.emplace<Speed>(player, 200.0f);
    registry.emplace<Health>(player, 10000.0f, 10000.0f);
    registry.emplace<Attack>(player, 100.0f);
    registry.emplace<Hitbox>(player, Hitbox{25.0f, 20.0f, 0.0f, 0.0f});
    registry.emplace<MovementDirection>(player, 0.0f, 0.0f);
    registry.emplace<LookingDirection>(player, 0.0f, 0.0f);
    registry.emplace<Mana>(player, 1000000.0f);
    registry.emplace<RepelResistance>(player, 0.5f);

    sf::Texture* mageTexture = TextureManager::getInstance().getTexture("Mage");

	StateComponent stateComp;
	stateComp.currentState = EntityState::Idle;
	stateComp.currentDirection = Direction::Down;
	stateComp.previousState = EntityState::Idle;
	stateComp.previousDirection = Direction::Down;
	stateComp.timer = 0.0f;
	stateComp.duration = -1.0f; // Infinite duration for idle state
	registry.emplace<StateComponent>(player, stateComp);

    AnimationComponent animComp;
    animComp.name = "Mage";
    animComp.currentFrame = { 0, 0 };
    animComp.timer = 0.0f;
    registry.emplace<AnimationComponent>(player, animComp);

 //   // sprite gắn vào để render
 //   sf::IntRect textureRect({ 0, 0 }, { 32, 48}); // Assuming each frame is 50x50 pixels
 //   sf::Sprite sprite(*mageTexture);
	//sprite.setTextureRect(textureRect);
	//sprite.setOrigin({ 16.f, 24.f }); // Set origin to center of the sprite
 //   sprite.setPosition({ 0.f, 0.f });

 //   registry.emplace<sf::Sprite>(player, sprite);	
    return player;
}