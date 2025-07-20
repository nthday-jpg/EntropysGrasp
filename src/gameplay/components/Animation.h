#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

enum class Direction {
	Up,
	Down,
	Left,
	Right,
	UpRight,
	UpLeft,
	DownRight,
	DownLeft,
};

enum class AnimationState {
	Idle,
	Walking,
	Attacking,
	Dashing,
	Dead,
};

struct Animation {
	sf::Vector2u frameCount; // Number of frames in the animation
	sf::Vector2u startFrame; // Starting frame in the sprite sheet
	sf::Texture* texture;
	float frameDuration;
};

struct AnimationData {
	std::map<std::pair<AnimationState, Direction>, Animation> animations;
};

struct AnimationComponent { // được gắn vào entity
	AnimationData* data; 
	AnimationState currentState;
	Direction currentDirection;
	sf::Vector2u currentFrame;
	float timer;
};