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

void AnimationManager::loadAnimationDatabase(const std::string& name, int i) {
}

void AnimationManager::loadAnimationData() {
	animationDatabase["Mage"] = loadMageAnimations();
	animationDatabase["orc1"] = loadOrcAnimations(1);
	animationDatabase["orc2"] = loadOrcAnimations(2);
	animationDatabase["orc3"] = loadOrcAnimations(3);
	animationDatabase["slime1"] = loadSlimeAnimations(1);
	animationDatabase["slime2"] = loadSlimeAnimations(2);
	animationDatabase["slime3"] = loadSlimeAnimations(3);
	for (int i = 0; i < 9; ++i) {
		std::string name1 = "spell1" + std::to_string(i);
		std::string name2 = "spell2" + std::to_string(i);
		std::string name3 = "spell3" + std::to_string(i);
		animationDatabase[name1] = loadSpellAnimations("spell1", i);
		animationDatabase[name2] = loadSpellAnimations("spell2", i);
		animationDatabase[name3] = loadSpellAnimations("spell3", i);
	}
	animationDatabase["bolt"] = loadBoltSpellAnimations();
	animationDatabase["charged"] = loadChargedSpellAnimations();
	animationDatabase["cross"] = loadCrossSpellAnimations();
	animationDatabase["pulse"] = loadPulseSpellAnimations();
	animationDatabase["spark"] = loadSparkSpellAnimations();
	animationDatabase["wave"] = loadWaveSpellAnimations();
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
	const int framesPerRow = textureOut->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;

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
	const float frameDuration = 0.1f;
	auto add = [&](sf::Texture* texture,AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = texture;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(texture->getSize().x / frameSize.x, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	add(textureIdle, AnimationState::Idle, (Direction)0, 0);
	add(textureWalk, AnimationState::Walking, (Direction)0, 0);
	add(textureAttack, AnimationState::Attacking, (Direction)0, 0);
	add(textureDeath, AnimationState::Dead, (Direction)0, 0);
	add(textureIdle, AnimationState::Idle, (Direction)4, 1);
	add(textureWalk, AnimationState::Walking, (Direction)4, 1);
	add(textureAttack, AnimationState::Attacking, (Direction)4, 1);
	add(textureDeath, AnimationState::Dead, (Direction)4, 1);
	add(textureIdle, AnimationState::Idle, (Direction)2, 2);
	add(textureWalk, AnimationState::Walking, (Direction)2, 2);
	add(textureAttack, AnimationState::Attacking, (Direction)2, 2);
	add(textureDeath, AnimationState::Dead, (Direction)2, 2);
	add(textureIdle, AnimationState::Idle, (Direction)6, 3);
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
	const float frameDuration = 0.1f;
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

AnimationData AnimationManager::loadSpellAnimations(const std::string& name, int color) {
	AnimationData data;
 
	sf::Texture* textureCast = TextureManager::getInstance().getTexture(name);
	const sf::Vector2i frameSize = { 64, 64 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	add(AnimationState::Attacking, Direction::Down, color);
	return data;
}

AnimationData AnimationManager::loadBoltSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("bolt");
	const sf::Vector2i frameSize = { 58, 58 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	int a = 0;
	for (int i = 10; i > 2; --i) {
		add(AnimationState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}

AnimationData AnimationManager::loadChargedSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("charged");
	const sf::Vector2i frameSize = { 79, 80 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	int a = 0;
	for (int i = 14; i > 6; --i) {
		add(AnimationState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}

AnimationData AnimationManager::loadCrossSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("cross");
	const sf::Vector2i frameSize = { 46, 46 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	int a = 0;
	for (int i = 14; i > 6; --i) {
		add(AnimationState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}


AnimationData AnimationManager::loadPulseSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("pulse");
	const sf::Vector2i frameSize = { 69, 68 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	int a = 0;
	for (int i = 14; i > 6; --i) {
		add(AnimationState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}

AnimationData AnimationManager::loadSparkSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("spark");
	const sf::Vector2i frameSize = { 69, 68 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	int a = 0;
	for (int i = 14; i > 6; --i) {
		add(AnimationState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}

AnimationData AnimationManager::loadWaveSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("wave");
	const sf::Vector2i frameSize = { 95, 95 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](AnimationState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	int a = 0;
	for (int i = 14; i > 6; --i) {
		add(AnimationState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}