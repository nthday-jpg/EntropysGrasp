#pragma once
#include "../Scene.h"
#include "../../control/GameplayInputHandler.h"
#include "../../control/GameplayCommandManager.h"
#include <entt/entity/registry.hpp>

class GameplayScene : public Scene {
private:
	//Mapmanager* mapManager;
    GameplayCommandManager* gameplayCommandManager;
    GameplayInputHandler* inputHandler;
    
	bool isPaused = false;
    
public:
    GameplayScene(sf::RenderWindow& window);
    ~GameplayScene();
    
    void load() override;
    bool handleEvent(const std::optional<sf::Event>& event) override;
    void update(float deltaTime) override;
    void render() override;

	void pause();
    void resume();
	void restart();
	void exit();
    
private:

};