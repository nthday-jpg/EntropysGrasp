#include "MovementPipeline.h"

MovementPipeline::MovementPipeline(entt::registry& registry) : registry(registry) {}

void MovementPipeline::update(float dt)
{
    PlayerVelocityController::calculateVelocity(registry);

    BehaviorSystem::getInstance().updateBehavior(registry, dt);

    PositionIntegrator::update(registry, dt);

}