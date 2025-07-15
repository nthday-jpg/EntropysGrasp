#pragma once
#include "../components/Particle.h"
#include "../components/MovementComponents.h"
#include <functional>
#include <entt/entity/entity.hpp>

using ParticleBehaviorFunction = std::function<void(entt::entity, ParticleComponent&, Position&, Velocity&, float)>;

class Linear {
public:
    static ParticleBehaviorFunction create();
};

class Floating {
public:
    static ParticleBehaviorFunction create();
};

class Orbit {
public:
    static ParticleBehaviorFunction create();
};

//class GravityBehavior {
//public:
//    static ParticleBehaviorFunction create();
//};
//
//class SpiralBehavior {
//public:
//    static ParticleBehaviorFunction create();
//};
//
//class BurstBehavior {
//public:
//    static ParticleBehaviorFunction create();
//};

