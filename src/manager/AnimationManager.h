#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "../gameplay/components/Animation.h"


class AnimationManager {
private:
	AnimationManager(const AnimationManager&) = delete;
	AnimationManager& operator=(const AnimationManager&) = delete;
	~AnimationManager() = default;
public:
	std::unordered_map<std::string, AnimationData> animationDatabase;

	AnimationManager();
	static AnimationManager& getInstance();
	void loadAnimationData();
	AnimationData* getAnimationData(const std::string& name);
	AnimationData loadMageAnimations();
	AnimationData loadOrcAnimations(int a);
	AnimationData loadSlimeAnimations(int a);
	AnimationData loadSpellAnimations();
};
