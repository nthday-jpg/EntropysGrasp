#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

enum class Direction {
	Down, // 0
	DownLeft, // 1
	Left, // 2
	UpLeft, // 3
	Up, // 4
	DownRight, // 5
	Right, // 6
	UpRight, // 7
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
	sf::Vector2f frameSize; // Size of each frame in the sprite sheet
	sf::Vector2u startFrame; // Starting frame in the sprite sheet
	sf::Texture* texture;
	float frameDuration = 1.0f / 60;
};

struct AnimationData {
	std::map<std::pair<AnimationState, Direction>, Animation> animations;
};

struct AnimationComponent { // được gắn vào entity
	std::string name; // tên animation
	AnimationState currentState;
	Direction currentDirection;
	sf::Vector2u currentFrame;
	float timer;
};