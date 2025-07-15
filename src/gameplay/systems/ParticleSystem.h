#pragma once
#include "../components/Particle.h"
#include "ParticleBehaviors.h"
#include <entt/entity/registry.hpp>
#include <entt/entity/group.hpp>
#include <unordered_map>


class ParticleSystem
{
	entt::registry& registry;
	std::unordered_map<ParticleBehaviorType, ParticleBehaviorFunction> behaviorMap;

	void initializeBehaviorMap();

public:
	ParticleSystem(entt::registry& registry);

	void emit(const ParticleProperties& properties);
	void update(float dt);
	void render();
};

