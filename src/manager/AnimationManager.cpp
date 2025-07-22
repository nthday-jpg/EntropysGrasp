#include "AnimationManager.h"

AnimationManager& AnimationManager::getInstance() {
	static AnimationManager instance;
	return instance;
}

void AnimationManager::loadAnimationData(const std::string& name, const std::string& spritePath, sf::Texture* texture) {
	animationDatabase[name] = loadMageAnimations(spritePath, *texture);
}

AnimationData* AnimationManager::getAnimationData(const std::string& name) {
	auto it = animationDatabase.find(name);
	if (it != animationDatabase.end())
		return &(it->second);
	return nullptr;
}

AnimationData AnimationManager::loadMageAnimations(const std::string& spritePath, sf::Texture& textureOut) {
	AnimationData data;

	if (!textureOut.loadFromFile(spritePath)) {
		throw std::runtime_error("Failed to load Mage-Sheet.png");
	}

	const sf::Vector2i frameSize = { 32, 48 }; // Mỗi frame là 64x64
	const int framesPerRow = 6;
	const float frameDuration = 0.03f;

	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = &textureOut;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};

	for (int i = 0; i < 8; i++) {
		// Idle animations
		add(AnimationState::Idle, (Direction)i, 3*i);
		add(AnimationState::Walking, (Direction)i, 3*i + 1);
		add(AnimationState::Attacking, (Direction)i, 3*i + 2);
	}

	// (Có thể thêm các trạng thái khác như Attack, Cast...)

	return data;
}