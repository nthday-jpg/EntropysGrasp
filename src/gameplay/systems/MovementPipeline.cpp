#include "MovementPipeline.h"

MovementPipeline::MovementPipeline(entt::registry& registry) : registry(registry) {}

void MovementPipeline::update(float dt)
{
    PlayerVelocityController::calculateVelocity(registry);

    behaviorSystem.updateBehavior(registry, dt);

    PositionIntegrator::update(registry, dt);

}