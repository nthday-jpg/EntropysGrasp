#include "StateSystem.h"
#include "../components/EntityTags.h"
#include <iostream>

    void StateSystem::onStateChangeState(const StateChangeEventState& event)
    {
        if (!registry.valid(event.entity) || !registry.all_of<StateComponent>(event.entity)) return;

        StateComponent& state = registry.get<StateComponent>(event.entity);

	    if (state.currentState == event.newState) return; 

	    if (state.duration - state.timer > 0) return;

        state.previousState = state.currentState;
        state.currentState = event.newState;
        state.timer = 0.f;
        state.duration = event.duration;
    }

    void StateSystem::onStateChangeDirection(const StateChangeEventDirection& event)
    {
        if (!registry.valid(event.entity) || !registry.all_of<StateComponent>(event.entity)) return;
        StateComponent& state = registry.get<StateComponent>(event.entity);

	    if (state.currentDirection == event.newDirection) return;

	    if (state.duration - state.timer > 0) return;

        state.previousDirection = state.currentDirection;
        state.currentDirection = event.newDirection;
    }

    void StateSystem::update(float dt) {
        auto view = registry.view<StateComponent>();
        for (auto entity : view) {
            auto& state = view.get<StateComponent>(entity);

            if (state.duration > 0) 
            {
                state.timer += dt;
                if (state.timer >= state.duration) 
                {
                    if (entt::dispatcher* dispatcher = *registry.ctx().find<entt::dispatcher*>()) 
                    {
					    dispatcher->trigger<StateChangeEventState>({ entity, state.previousState, -1.0f });
                    }
                }
            }
        }
    }

    void StateSystem::sinkEvents() 
    {
        // Access dispatcher from registry context and connect to events
        if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>())
        {
            (*dispatcher)->sink<StateChangeEventState>().connect<&StateSystem::onStateChangeState>(*this);
            (*dispatcher)->sink<StateChangeEventDirection>().connect<&StateSystem::onStateChangeDirection>(*this);
        }
    }