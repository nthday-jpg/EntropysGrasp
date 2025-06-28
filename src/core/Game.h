#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "../Utils/SpatialHash.h"
//Include thêm các hệ thống khác ở đây
#include "../systems/PositionIntegrator.h"
#include "../systems/CollisionSystem.h"

//Hằng số
const int windowHeight = 600;
const int windowWidth = 800;
const int FPS = 60;

class Game {
public:
    Game();
	~Game();
    void run();

    entt::registry& getRegistry() { return registry; }
    entt::dispatcher& getDispatcher() { return dispatcher; }

private:
    void processInput();
	void update(float dt);
	void render();

    sf::RenderWindow window;
	sf::View cameraView; 

    entt::registry registry;
    entt::dispatcher dispatcher;
    
	//Resource management
    //Game state
	//Map management

	sf::Clock gameClock; 
    float timestep = 1.0f / FPS;
	float accumulator = 0.0f; 
};
