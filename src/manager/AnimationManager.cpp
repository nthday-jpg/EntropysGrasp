#include "AnimationManager.h"
#include "TextureManager.h"
#include <string>
#include <iostream>

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
		std::string nameCircle = "spellCircle" + std::to_string(i);
		animationDatabase[name1] = loadSpellAnimations("spell1", i);
		animationDatabase[name2] = loadSpellAnimations("spell2", i);
		animationDatabase[name3] = loadSpellAnimations("spell3", i);
		animationDatabase[nameCircle] = loadSpellCircleAnimations(i);
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

	auto add = [&](EntityState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureOut;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};

	
	add(EntityState::Idle, Direction::Down, 0);
	add(EntityState::Walking, Direction::Down, 1);
	add(EntityState::Attacking, Direction::Down, 2);
	add(EntityState::Idle, Direction::DownLeft, 3);
	add(EntityState::Walking, Direction::DownLeft, 4);
	add(EntityState::Attacking, Direction::DownLeft, 5);
	add(EntityState::Idle, Direction::Left, 6);
	add(EntityState::Walking, Direction::Left, 7);
	add(EntityState::Attacking, Direction::Left, 8);
	add(EntityState::Idle, Direction::UpLeft, 9);
	add(EntityState::Walking, Direction::UpLeft, 10);
	add(EntityState::Attacking, Direction::UpLeft, 11);
	add(EntityState::Idle, Direction::Up, 12);
	add(EntityState::Walking, Direction::Up, 13);
	add(EntityState::Attacking, Direction::Up, 14);
	add(EntityState::Idle, Direction::DownRight, 15);
	add(EntityState::Walking, Direction::DownRight, 16);
	add(EntityState::Attacking, Direction::DownRight, 17);
	add(EntityState::Idle, Direction::Right, 18);
	add(EntityState::Walking, Direction::Right, 19);
	add(EntityState::Attacking, Direction::Right, 20);
	add(EntityState::Idle, Direction::UpRight, 21);
	add(EntityState::Walking, Direction::UpRight, 22);
	add(EntityState::Attacking, Direction::UpRight, 23);

	return data;
}

AnimationData AnimationManager::loadOrcAnimations(int a) {
	AnimationData data;
	std::string name = "orc" + std::to_string(a);
	sf::Texture* texture = TextureManager::getInstance().getTexture(name);

	const sf::Vector2i frameSize = { 64, 64 };
	const float frameDuration = 0.1f;

	// Mapping: state + direction -> row
	// Giả sử bạn gộp theo thứ tự: Idle, Walk, Attack, Death
	// và mỗi state có 4 row: Down, Up, Left, Right
	// => Idle: rows 0-3, Walk: rows 4-7, Attack: rows 8-11, Death: rows 12-15
	auto add = [&](EntityState state, Direction dir, int row, int framePerRow) {
		Animation anim;
		anim.texture = texture;
		anim.frameSize = sf::Vector2f((float)frameSize.x, (float)frameSize.y);
		anim.frameCount = sf::Vector2u(framePerRow, 1);
		anim.startFrame = sf::Vector2u(0u, (unsigned int)row);
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};

	// Idle
	add(EntityState::Idle, Direction::Down, 0, 4);
	add(EntityState::Idle, Direction::Up, 1, 4);
	add(EntityState::Idle, Direction::Left, 2, 4);
	add(EntityState::Idle, Direction::Right, 3, 4);

	// Walk
	add(EntityState::Walking, Direction::Down, 12, 8);
	add(EntityState::Walking, Direction::Up, 13, 8);
	add(EntityState::Walking, Direction::Left, 14, 8);
	add(EntityState::Walking, Direction::Right, 15, 8);

	// Attack
	add(EntityState::Attacking, Direction::Down, 4, 8);
	add(EntityState::Attacking, Direction::Up, 5, 8);
	add(EntityState::Attacking, Direction::Left, 6, 8);
	add(EntityState::Attacking, Direction::Right, 7, 8);

	// Death
	add(EntityState::Dead, Direction::Down, 8, 8);
	add(EntityState::Dead, Direction::Up, 9, 8);
	add(EntityState::Dead, Direction::Left, 10, 8);
	add(EntityState::Dead, Direction::Right, 11, 8);

	return data;
}

AnimationData AnimationManager::loadSlimeAnimations(int a) {
	AnimationData data;
	std:: string name = "slime" + std::to_string(a);
	sf::Texture* texture = TextureManager::getInstance().getTexture(name);
	const sf::Vector2i frameSize = { 64, 64 };
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row, int framePerRow) {
		Animation anim;
		anim.texture = texture;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framePerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};

	add(EntityState::Idle, Direction::Down, 0, 6);
	add(EntityState::Idle, Direction::Up, 1, 6);
	add(EntityState::Idle, Direction::Left, 2, 6);
	add(EntityState::Idle, Direction::Right, 3, 6);

	add(EntityState::Walking, Direction::Down, 12, 10);
	add(EntityState::Walking, Direction::Up, 13, 10);
	add(EntityState::Walking, Direction::Left, 14, 10);
	add(EntityState::Walking, Direction::Right, 15, 10);

	add(EntityState::Attacking, Direction::Down, 4, 10);
	add(EntityState::Attacking, Direction::Up, 5, 10);
	add(EntityState::Attacking, Direction::Left, 6, 10);
	add(EntityState::Attacking, Direction::Right, 7, 10);

	add(EntityState::Dead, Direction::Down, 8, 8);
	add(EntityState::Dead, Direction::Up, 9, 8);
	add(EntityState::Dead, Direction::Left, 10, 8);
	add(EntityState::Dead, Direction::Right, 11, 8);

	return data;
}

AnimationData AnimationManager::loadSpellAnimations(const std::string& name, int color) {
	AnimationData data;
 
	sf::Texture* textureCast = TextureManager::getInstance().getTexture(name);
	const sf::Vector2i frameSize = { 64, 64 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	add(EntityState::Attacking, Direction::Down, color);
	add(EntityState::Attacking, Direction::Up, color);
	add(EntityState::Attacking, Direction::Left, color);
	add(EntityState::Attacking, Direction::Right, color);
	add(EntityState::Attacking, Direction::DownLeft, color);
	add(EntityState::Attacking, Direction::DownRight, color);
	add(EntityState::Attacking, Direction::UpLeft, color);
	add(EntityState::Attacking, Direction::UpRight, color);

	return data;
}

AnimationData AnimationManager::loadSpellCircleAnimations(int color) {
	AnimationData data;

	sf::Texture* textureCast = TextureManager::getInstance().getTexture("spell_circle");
	const sf::Vector2i frameSize = { 64, 64 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row) {
		Animation anim;
		anim.texture = textureCast;
		anim.frameSize = sf::Vector2f(static_cast<float>(frameSize.x), static_cast<float>(frameSize.y));
		anim.frameCount = sf::Vector2u(framesPerRow, 1);
		anim.startFrame = sf::Vector2u(0u, static_cast<unsigned int>(row));
		anim.frameDuration = frameDuration;
		data.animations[{ state, dir }] = anim;
		};
	add(EntityState::Attacking, Direction::Down, color);
	add(EntityState::Attacking, Direction::Up, color);
	add(EntityState::Attacking, Direction::Left, color);
	add(EntityState::Attacking, Direction::Right, color);
	add(EntityState::Attacking, Direction::DownLeft, color);
	add(EntityState::Attacking, Direction::DownRight, color);
	add(EntityState::Attacking, Direction::UpLeft, color);
	add(EntityState::Attacking, Direction::UpRight, color);

	return data;
}


AnimationData AnimationManager::loadBoltSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("bolt");
	const sf::Vector2i frameSize = { 58, 58 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row) {
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
		add(EntityState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}

AnimationData AnimationManager::loadChargedSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("charged");
	const sf::Vector2i frameSize = { 79, 80 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row) {
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
		add(EntityState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}

AnimationData AnimationManager::loadCrossSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("cross");
	const sf::Vector2i frameSize = { 46, 46 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row) {
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
		add(EntityState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}


AnimationData AnimationManager::loadPulseSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("pulse");
	const sf::Vector2i frameSize = { 69, 68 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row) {
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
		add(EntityState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}

AnimationData AnimationManager::loadSparkSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("spark");
	const sf::Vector2i frameSize = { 69, 68 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row) {
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
		add(EntityState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}

AnimationData AnimationManager::loadWaveSpellAnimations() {
	AnimationData data;
	sf::Texture* textureCast = TextureManager::getInstance().getTexture("wave");
	const sf::Vector2i frameSize = { 95, 95 };
	const int framesPerRow = textureCast->getSize().x / frameSize.x;
	const float frameDuration = 0.1f;
	auto add = [&](EntityState state, Direction dir, int row) {
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
		add(EntityState::Attacking, (Direction)(i % 8), a++);
	}
	return data;
}