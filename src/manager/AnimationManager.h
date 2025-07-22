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

	AnimationManager() = default;
	static AnimationManager& getInstance();
	void loadAnimationData(const std::string& name, const std::string& spritePath, sf::Texture* texture);
	AnimationData* getAnimationData(const std::string& name);
	AnimationData loadMageAnimations(const std::string& spritePath, sf::Texture& textureOut);
};
