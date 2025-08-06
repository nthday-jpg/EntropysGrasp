#pragma once
#include "PositionIntegrator.h"
#include "BehaviorSystem.h"
#include "PlayerVelocitySystem.h"
#include "../../manager/SpellLibrary.h"
#include "../../manager/EnemyLibrary.h"

// Orchestrates the flow of the movement update sequence by coordinating
// multiple specialized movement systems
class MovementPipeline {
private:
	BehaviorSystem behaviorSystem;
    entt::registry& registry;
public:
    MovementPipeline(entt::registry& registry);

    void update(float dt);
};