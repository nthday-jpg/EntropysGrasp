#include "AnimationManager.h"
#include "TextureManager.h"
#include <string>

AnimationManager& AnimationManager::getInstance() {
	static AnimationManager instance;
	return instance;
}

AnimationManager::AnimationManager() {
	loadAnimationData();
}

void AnimationManager::loadAnimationData() {
	animationDatabase["Mage"] = loadMageAnimations();
	animationDatabase["orc1"] = loadOrcAnimations(1);
	animationDatabase["orc2"] = loadOrcAnimations(2);
	animationDatabase["orc3"] = loadOrcAnimations(3);
	animationDatabase["slime1"] = loadSlimeAnimations(1);
	animationDatabase["slime2"] = loadSlimeAnimations(2);
	animationDatabase["slime3"] = loadSlimeAnimations(3);
	animationDatabase["spell2"] = loadSpellAnimations();
}

AnimationData* AnimationManager::getAnimationData(const std::string& name) {
	auto it = animationDatabase.find(name);
	if (it != animationDatabase.end())
		return &(it->second);
	return nullptr;
}

AnimationData AnimationManager::loadMageAnimations() {
	AnimationData data;

	std::string name = "Mage";
	sf::Texture* textureOut = TextureManager::getInstance().getTexture(name);

	const sf::Vector2i frameSize = { 32, 48 };
	const int framesPerRow = 6;
	const float frameDuration = 0.03f;

	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureOut;
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

AnimationData AnimationManager::loadOrcAnimations(int a) {
	AnimationData data;
	std::string name = "orc" + std::to_string(a);
	sf::Texture* textureAttack = TextureManager::getInstance().getTexture(name + "_attack");
	sf::Texture* textureIdle = TextureManager::getInstance().getTexture(name + "_idle");
	sf::Texture* textureWalk = TextureManager::getInstance().getTexture(name + "_walk");
	sf::Texture* textureDeath = TextureManager::getInstance().getTexture(name + "_death");
	const sf::Vector2i frameSize = { 64, 64 };
	const float frameDuration = 0.03f;
	auto add = [&](sf::Texture* texture,AnimationState state, Direction dir, int row, int framesPerRow = 8) {
		Animation anim;
		anim.texture = texture;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	add(textureIdle, AnimationState::Idle, (Direction)0, 0, 4);
	add(textureWalk, AnimationState::Walking, (Direction)0, 0);
	add(textureAttack, AnimationState::Attacking, (Direction)0, 0);
	add(textureDeath, AnimationState::Dead, (Direction)0, 0);
	add(textureIdle, AnimationState::Idle, (Direction)4, 1, 4);
	add(textureWalk, AnimationState::Walking, (Direction)4, 1);
	add(textureAttack, AnimationState::Attacking, (Direction)4, 1);
	add(textureDeath, AnimationState::Dead, (Direction)4, 1);
	add(textureIdle, AnimationState::Idle, (Direction)2, 2 , 4);
	add(textureWalk, AnimationState::Walking, (Direction)2, 2);
	add(textureAttack, AnimationState::Attacking, (Direction)2, 2);
	add(textureDeath, AnimationState::Dead, (Direction)2, 2);
	add(textureIdle, AnimationState::Idle, (Direction)6, 3, 4);
	add(textureWalk, AnimationState::Walking, (Direction)6, 3);
	add(textureAttack, AnimationState::Attacking, (Direction)6, 3);
	add(textureDeath, AnimationState::Dead, (Direction)6, 3);

	return data;
}

AnimationData AnimationManager::loadSlimeAnimations(int a) {
	AnimationData data;
	std:: string name = "slime" + std::to_string(a);
	sf::Texture* textureAttack = TextureManager::getInstance().getTexture(name + "_attack");
	sf::Texture* textureIdle = TextureManager::getInstance().getTexture(name + "_idle");
	sf::Texture* textureWalk = TextureManager::getInstance().getTexture(name + "_walk");
	sf::Texture* textureDeath = TextureManager::getInstance().getTexture(name + "_death");
	const sf::Vector2i frameSize = { 64, 64 };
	const float frameDuration = 0.03f;
	auto add = [&](sf::Texture* texture, AnimationState state, Direction dir, int row, int framesPerRow) {
		Animation anim;
		anim.texture = texture;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	add(textureIdle, AnimationState::Idle, (Direction)0, 0, 6);
	add(textureWalk, AnimationState::Walking, (Direction)0, 0, 8);
	add(textureAttack, AnimationState::Attacking, (Direction)0, 0, 9);
	add(textureDeath, AnimationState::Dead, (Direction)0, 0, 10);
	add(textureIdle, AnimationState::Idle, (Direction)4, 1, 6);
	add(textureWalk, AnimationState::Walking, (Direction)4, 1, 8);
	add(textureAttack, AnimationState::Attacking, (Direction)4, 1, 9);
	add(textureDeath, AnimationState::Dead, (Direction)4, 1, 10);
	add(textureIdle, AnimationState::Idle, (Direction)2, 2, 6);
	add(textureWalk, AnimationState::Walking, (Direction)2, 2, 8);
	add(textureAttack, AnimationState::Attacking, (Direction)2, 2, 9);
	add(textureDeath, AnimationState::Dead, (Direction)2, 2, 10);
	add(textureIdle, AnimationState::Idle, (Direction)6, 3, 6);
	add(textureWalk, AnimationState::Walking, (Direction)6, 3, 8);
	add(textureAttack, AnimationState::Attacking, (Direction)6, 3, 9);
	add(textureDeath, AnimationState::Dead, (Direction)6, 3, 10);

	return data;
}

AnimationData AnimationManager::loadSpellAnimations() {
	AnimationData data;
	std::string name = "spell2";
	sf::Texture* textureCast = TextureManager::getInstance().getTexture(name);
	const sf::Vector2i frameSize = { 64, 64 };
	const int framesPerRow = 14;
	const float frameDuration = 0.03f;
	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	add(AnimationState::Attacking, Direction::Down, 0);
	return data;
}