#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "State.h"

struct Animation {
	sf::Vector2u frameCount; // Number of frames in the animation
	sf::Vector2f frameSize; // Size of each frame in the sprite sheet
	sf::Vector2u startFrame; // Starting frame in the sprite sheet
	sf::Texture* texture;
	float frameDuration = 1.0f / 60;
};

struct AnimationData {
	std::map<std::pair<EntityState, Direction>, Animation> animations;
};

struct AnimationComponent { // được gắn vào entity
	std::string name; // tên animation
	EntityState currentState;
	Direction currentDirection;
	sf::Vector2u currentFrame;
	float timer;
};