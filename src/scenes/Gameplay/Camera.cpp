#include "Camera.h"
#include "../../GameConfig.h"
#include "../../gameplay/components/MovementComponents.h"

Camera::Camera(entt::registry* registry) : registry(registry)
{
	randomEngine = std::mt19937(std::random_device{}());

	GameConfig& config = GameConfig::getInstance();
	size = sf::Vector2f(config.getWindowWidth(), config.getWindowHeight());
	view.setSize(size);

	basePosition = sf::Vector2f(size.x / 2.0f, size.y / 2.0f);
	view.setCenter(basePosition);
}

void Camera::setPosition(const sf::Vector2f& position)
{
	basePosition = position;
	view.setCenter(basePosition);
}

void Camera::setSize(sf::Vector2f newSize)
{
	size = newSize;
	view.setSize(size);
}

void Camera::move(sf::Vector2f offset)
{
	basePosition += offset;
	view.setCenter(basePosition);
}

void Camera::startShake(float intensity, float duration)
{
	this->shakeDuration = duration;
	shakeIntensity = intensity;
	shakeTimer = 0.0f;
}


bool Camera::isShaking() const
{
	return shakeIntensity > 0.0f && shakeTimer < shakeDuration;
}

void Camera::update(float dt)
{
	if (isFollowing && registry)
	{
		Position& targetPos = registry->get<Position>(followTarget);
		sf::Vector2f diff = targetPos - basePosition + followOffset;
		basePosition += diff * followSmoothness * dt;
	}
	else
	{
		stopFollowing();
	}

	if (isShaking())
	{
		shakeTimer += dt;
		if (shakeTimer >= shakeDuration)
		{
			shakeIntensity = 0.0f;
			shakeTimer = 0.0f;
			view.setCenter(basePosition);
		}
		else
		{
			float progress = shakeTimer / shakeDuration;
			float currentIntensity = shakeIntensity * (1.0f - progress); // Fade out

			sf::Vector2f shakeOffset(
				distribution(randomEngine) * currentIntensity,
				distribution(randomEngine) * currentIntensity
			);

			view.setCenter(basePosition + shakeOffset);
		}
	}
	else
	{
		view.setCenter(basePosition);
	}

}

void Camera::followEntity(entt::entity entity)
{
	this->followTarget = entity;
	if (followTarget != entt::null)
	{
		isFollowing = true;
	}
	else
	{
		isFollowing = false;
	}
}

void Camera::stopFollowing()
{
	this->isFollowing = false;
	this->followTarget = entt::null;
}

void Camera::setFollowOffset(sf::Vector2f offset)
{
	this->followOffset = offset;
}

const sf::View& Camera::getView() const
{
	return view;
}