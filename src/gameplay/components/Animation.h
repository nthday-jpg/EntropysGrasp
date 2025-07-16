#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

enum class AnimationState {
	Idle,
	Walking,
	Attacking,
	Dashing,
	Dead,
};

struct Animation {
	sf::Vector2u frameCount; // Number of frames in the animation
	sf::Vector2u currentFrame; // Current frame index
	float frameDuration; // Duration for each frame in seconds
	sf::Texture* texture; // Texture for the animation
};