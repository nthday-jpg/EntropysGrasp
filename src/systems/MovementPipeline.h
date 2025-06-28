#pragma once
#include "PositionIntegrator.h"
#include "BehaviorSystem.h"
#include "../behaviors/PlayerVelocityController.h"
#include "../resources/SpellLibrary.h"
#include "../resources/EnemyLibrary.h"

// Orchestrates the flow of the movement update sequence by coordinating
// multiple specialized movement systems
class MovementPipeline {
private:
    BehaviorSystem behaviorSystem;
    const SpellLibrary& spellLibrary;
    const EnemyLibrary& enemyLibrary;

public:
    MovementPipeline(const SpellLibrary& spellLib, const EnemyLibrary& enemyLib);

    void update(entt::registry& registry, float dt);
};