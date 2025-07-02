#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <entt/entity/registry.hpp>
#include "../systems/WindowManager.h"
#include "../control/commandManager.h"
#include "../ui/UIManager.h"

class Scene {
protected:
    entt::registry registry;
    GameplayCommandManager commandManager;
    UIManager* uiManager;
    WindowManager& windowManager = WindowManager::getInstance();

public:
    Scene() :
        commandManager(registry),
        uiManager(new UIManager(&commandManager)) {
    }

    virtual ~Scene()
    {
		delete uiManager;
    }

    // Called when scene becomes active
    virtual void onEnter() {}

    // Called when scene is no longer active
    virtual void onExit() {}

    // Process input (both gameplay and UI)
    virtual void handleInput() = 0;

    // Update game logic
    virtual void update(float deltaTime) = 0;

    // Render the scene
    virtual void render() = 0;

    // Registry access
    entt::registry& getRegistry() { return registry; }

    // UI access
    UIManager& getUI() { return *uiManager; }
};