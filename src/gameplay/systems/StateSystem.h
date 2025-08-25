#pragma once
#include <entt/entt.hpp>
#include "../components/State.h"

struct StateChangeEventState {
    entt::entity entity;
    EntityState newState;
    float duration; // -1 = loop (Idle, Walk), >0 = temporary (Attack 0.8s, Dash 0.5s)
};

struct StateChangeEventDirection {
    entt::entity entity;
    Direction newDirection;
};


class StateSystem {
	entt::registry& registry;
public:
    StateSystem(entt::registry& registry) : registry(registry) {}
    void onStateChangeState(const StateChangeEventState& event);
	void onStateChangeDirection(const StateChangeEventDirection& event);
	void update(float deltaTime);
    void sinkEvents();
};