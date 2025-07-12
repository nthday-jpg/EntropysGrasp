#include "MovementPipeline.h"

MovementPipeline::MovementPipeline(const SpellLibrary& spellLib, const EnemyLibrary& enemyLib)
    : spellLibrary(spellLib), enemyLibrary(enemyLib) 
{
    behaviorSystem.initializeBehaviorMap();
}

void MovementPipeline::update(entt::registry& registry, float dt)
{
    PlayerVelocityController::calculateVelocity(registry);

    behaviorSystem.updateBehavior(registry, dt);

    PositionIntegrator::update(registry, dt);

}