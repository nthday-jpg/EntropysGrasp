#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <string>
#include "../components/Animation.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>

struct AnimationChangeEvent {
	entt::entity entity;
	AnimationState newState;
	Direction newDirection;
};

class AnimationSystem {
	entt::registry& registry;
	float frameDuration; // Duration for each frame
public:
	void update(float deltaTime); //tăng timer tăng currentframe reset timer
	void changeAnimation(const AnimationChangeEvent& event);
	void sinkEvents();
	AnimationSystem(entt::registry& registry);
};