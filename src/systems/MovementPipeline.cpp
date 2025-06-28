#include "MovementPipeline.h"

MovementPipeline::MovementPipeline(const SpellLibrary& spellLib, const EnemyLibrary& enemyLib)
    : spellLibrary(spellLib), enemyLibrary(enemyLib) 
{
    behaviorSystem.initializeBehaviorMap();
}

void MovementPipeline::update(entt::registry& registry, float dt)
{
    PlayerVelocityController::calculateVelo(registry);

	behaviorSystem.updateBehavior(registry, dt, spellLibrary, enemyLibrary);

    PositionIntegrator::update(registry, dt);

}