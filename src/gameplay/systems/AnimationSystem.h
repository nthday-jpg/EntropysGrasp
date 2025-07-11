#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <string>
#include "../components/Animation.h"

class AnimationSystem {
protected:
	entt::entity entityType;
	float frameDuration; // Duration for each frame
	float timer;
	int currentFrame; // Index of the current frame
	Animation& animation; // Animation data
public:
	
};