#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "MovementComponents.h"

enum class ParticleBehaviorType
{
	Linear,          // Straight line movement
	Spiral,          // Spiral movement
	Fade,            // Simple fade out
	Burst,           // Explosive outward movement
	Orbit,           // Orbit around a point
	Floating,        // Floating with random drift
	Attract          // Attracted to a target
};

struct ParticleProperties {
	Position position;
	Velocity velocity, velocityVariation;
	sf::Color startColor, endColor;
	float sizeStart, sizeEnd, sizeVariation;
	float lifetime = 1.0f;

	ParticleBehaviorType behaviorType = ParticleBehaviorType::Linear; // Default behavior
};

struct ParticleComponent {
	// position and velocity are handled by registry
	sf::Color colorStart, colorEnd;
	float beginSize, endSize;
	float lifetime;
	ParticleBehaviorType behaviorType = ParticleBehaviorType::Linear; // Default behavio

	float age = 0.0f;
};