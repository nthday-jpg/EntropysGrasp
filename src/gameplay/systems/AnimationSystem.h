#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <string>
#include "../components/Animation.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "StateSystem.h"

class AnimationSystem {
	entt::registry& registry;
	float frameDuration; // Duration for each frame
public:
	void update(float deltaTime); //tăng timer tăng currentframe reset timer
	//void changeAnimationState(const StateChangeEventState& event);
	//void changeAnimationDirection(const StateChangeEventDirection& event);
	//void sinkEvents();
	AnimationSystem(entt::registry& registry);
};