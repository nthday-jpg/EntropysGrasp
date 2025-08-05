#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <random>

class Camera
{
	sf::View view;
	sf::Vector2f basePosition;
	sf::Vector2f size;

	// Player following parameter
	entt::entity followTarget = entt::null;
	entt::registry* registry = nullptr;
	bool isFollowing = true;
	float followSmoothness = 0.1f; // Smoothness factor for following
	sf::Vector2f followOffset = { 0.0f, 0.0f };

	// Camera shake parameters
	float shakeIntensity = 0.0f;
	float shakeDuration = 0.0f;
	float shakeTimer = 0.0f;
	std::mt19937 randomEngine;
	std::uniform_real_distribution<float> distribution;

public:
	Camera(entt::registry* registry = nullptr);
	~Camera() = default;

	void setPosition(const sf::Vector2f& position);
	float getbaseX() const;
	float getbaseY() const;
	void setSize(sf::Vector2f size);
	sf::Vector2f getSize() const;
	void move(sf::Vector2f offset);

	void startShake(float intensity, float duration);
	void update(float dt);
	bool isShaking() const;

	void followEntity(entt::entity entity);
	void stopFollowing();
	void setFollowOffset(sf::Vector2f offset);

	const sf::View& getView() const;

};