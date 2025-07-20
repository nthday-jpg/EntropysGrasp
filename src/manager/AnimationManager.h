#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "../gameplay/components/Animation.h"


class AnimationManager {
private:
	AnimationManager() = default;
	AnimationManager(const AnimationManager&) = delete;
	AnimationManager& operator=(const AnimationManager&) = delete;
	~AnimationManager() = default;

public:
	std::unordered_map<std::string, AnimationData> animationDatabase;

	static AnimationManager& getInstance() {
		static AnimationManager instance;
		return instance;
	}

	void loadAnimationData(const std::string& name, const std::string& spritePath, sf::Texture* texture) {
		animationDatabase[name] = loadMageAnimations(spritePath, *texture);
	}

	AnimationData* getAnimationData(const std::string& name) {
		auto it = animationDatabase.find(name);
		if (it != animationDatabase.end())
			return &(it->second);
		return nullptr;
	}

	AnimationData loadMageAnimations(const std::string& spritePath, sf::Texture& textureOut) {
		AnimationData data;

		if (!textureOut.loadFromFile(spritePath)) {
			throw std::runtime_error("Failed to load Mage-Sheet.png");
		}

		const sf::Vector2i frameSize = { 64, 64 }; // Each sprite is 64x64
		const int framesPerRow = 8;
		const float frameDuration = 0.15f;

		auto add = [&](AnimationState state, Direction dir, int row) {
			Animation anim;
			anim.texture = &textureOut;
			anim.frameCount = sf::Vector2u(static_cast<unsigned int>(framesPerRow), 1u);
			anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
			anim.frameDuration = frameDuration;
			data.animations[{ state, dir }] = anim;
			};

		for (int i = 0; i < 4; ++i) {
			add(AnimationState::Idle, static_cast<Direction>(i), 0 + i);
			add(AnimationState::Walking, static_cast<Direction>(i), 4 + i);
			add(AnimationState::Attacking, static_cast<Direction>(i), 8 + i);
			add(AnimationState::Dead, static_cast<Direction>(i), 12 + i);
		}

		return data;
	}
};
