#include <SFML/System/Time.hpp>
#include <random>           
#include <iostream>
#include <string>
#include "../systems/EnemyManager.h"
#include "../components/EntityTags.h"
#include "../components/Enemy.h"
#include "../components/MovementComponents.h"
#include "../components/lookingDirection.h"
#include "../../scenes/Gameplay/Camera.h"
#include "../../utils/Random.h"
#include "../components/Hitbox.h"
#include "../../manager/TextureManager.h"
#include "../components/Animation.h"
#include "StateSystem.h"

const float PI = 3.14159265358979323846f;

Direction getDirectionFromMovementEnemy(const MovementDirection& movementDir) {
    float x = movementDir.x;
    float y = movementDir.y;
    float angle = std::atan2(y, x) * 180.0f / static_cast<float>(PI); // Convert to degrees
    if (angle <= 45.0f && angle > -45.0f) {
        return Direction::Right;
    }
    else if (angle > 45.0f && angle <= 135.0f) {
        return Direction::Down;
    }
    else if (angle > 135.0f || angle <= -135.0f) {
        return Direction::Left;
    }
    else {
        return Direction::Up;
    }
}


entt::entity EnemyManager::spawnEnemy(EnemyType type, Position position)
{
    const EnemyData& data = enemyLibrary.getEnemyData(type);

    float health = data.health.max * (1.0f + gameClock.getElapsedTime().asSeconds() * 0.01f);
    float speed = data.speed.value * (1.0f + gameClock.getElapsedTime().asSeconds() * 0.005f);
    float attack = data.attack.value * (1.0f + gameClock.getElapsedTime().asSeconds() * 0.005f);

    auto view = registry.view<Inactive, EnemyTag>();
    for (auto [entity] : view.each())
    {
        registry.remove<Inactive>(entity);
        registry.emplace_or_replace<EnemyType>(entity, type);
        registry.replace<Position>(entity, position.x, position.y);
        registry.replace<Health>(entity, health, health);
        registry.replace<Speed>(entity, speed);
        registry.replace<Attack>(entity, attack);
        return entity;
    }

    entt::entity entity = registry.create();
    registry.emplace<EnemyTag>(entity);
    registry.emplace<Position>(entity, position.x, position.y);
    registry.emplace<Health>(entity, health, health);
    registry.emplace<Speed>(entity, speed);
    registry.emplace<Attack>(entity, attack);
    registry.emplace<RepelResistance>(entity, data.resistance);
    registry.emplace<EnemyType>(entity, type);
    registry.emplace<BehaviorType>(entity, BehaviorType::HomingPlayer);
    registry.emplace<MovementDirection>(entity, 0.0f, 0.0f);
    registry.emplace<LookingDirection>(entity, 0.0f, 0.0f);
    registry.emplace<Hitbox>(entity, 40.0f, 35.0f, 0.0f, 0.0f); // Assuming a default hitbox size

    int a = Random::getInt(1, 3);

	StateComponent stateComp;
	stateComp.currentState = EntityState::Walking;
	stateComp.currentDirection = Direction::Down;
	stateComp.previousState = EntityState::Walking;
	stateComp.previousDirection = Direction::Down;
	stateComp.timer = 0.0f;
	stateComp.duration = -1.0f; // Infinite duration for walking state
	registry.emplace<StateComponent>(entity, stateComp);

    AnimationComponent animComp;
    if (type == EnemyType::Orc) 
        animComp.name = "orc" + std::to_string(a);
	else
        animComp.name = "slime" + std::to_string(a);
    animComp.currentFrame = { 0, 0 };
    animComp.timer = 0.0f;
    registry.emplace<AnimationComponent>(entity, animComp);

    //sf::Texture* texture = TextureManager::getInstance().getTexture("orc" + std::to_string(a) + "_walk");
    //sf::IntRect textureRect({ 0, 0 }, { 64, 64 });
    //sf::Sprite sprite(*texture);
    //sprite.setTextureRect(textureRect);
    //sprite.setOrigin({ 32.0f, 32.0f });
    //registry.emplace<sf::Sprite>(entity, sprite);

    return entity;
}

void EnemyManager::spawning(float dt)
{
    for (const auto& info : spawnInfos)
    {
        float& timer = spawnTimer[info.type];
        bool canSpawn = timer >= info.interval;
        if (canSpawn)
        {
            for (const auto& position : info.positions)
            {
                if (isInLoadChunk(position))
                {
                    Position randomPosition = randomizeOffScreenPosition(position);
                    spawnEnemy(info.type, randomPosition);
                }
            }
            timer = 0.0f; // Reset timer after spawning all positions for this type
        }
        timer += dt;
    }
}

void EnemyManager::removing()
{
    auto view = registry.view<EnemyTag>(entt::exclude<Inactive>);
    for (auto entity : view) {
        auto health = registry.try_get<Health>(entity);
        auto position = registry.try_get<Position>(entity);
        // Use for some entities that may not have health
        if (!health || !position)
        {
            continue;
        }
        if (health->current <= 0)
        {
            registry.emplace_or_replace<Inactive>(entity);
            // Reset position to a far away place
            registry.replace<Position>(entity, -1000.0f, -1000.0f);
            if (auto* dispatcher = registry.ctx().find<entt::dispatcher*>())
            {

                // Assuming there is only one player entity
                auto playerView = registry.view<PlayerTag>();
                entt::entity playerEntity = playerView.front();

                EnemyType enemyType = registry.get<EnemyType>(entity);
                Reward gold = { RewardType::Gold, static_cast<float>(getBaseGold(enemyType)), entt::null };
                Reward exp = { RewardType::Experience, static_cast<float>(getBaseExp(enemyType)), entt::null };
                Reward mana = { RewardType::Mana, getBaseMana(enemyType).value, playerEntity };
                (*dispatcher)->enqueue<Reward>(gold);
                (*dispatcher)->enqueue<Reward>(exp);
                (*dispatcher)->enqueue<Reward>(mana);
            }
        }
    }
}

bool EnemyManager::isInLoadChunk(const Position& position) const
{
    float dist = pow(camera.getbaseX() - position.x, 2) + pow(camera.getbaseY() - position.y, 2);

    return dist < loadChunkRadius;
}

void EnemyManager::update(float dt)
{

    spawning(dt);

    removing();

    auto view = registry.view<EnemyTag, MovementDirection, StateComponent, Position>(entt::exclude<Inactive>);
    for (auto [entity, movementDir, stateComp, position] : view.each()) {
        Direction newDirection = getDirectionFromMovementEnemy(movementDir);
        if (stateComp.currentDirection != newDirection)
        {
            if (entt::dispatcher* dispatcher = *registry.ctx().find<entt::dispatcher*>())
            {
                dispatcher->enqueue<StateChangeEventDirection>(entity, newDirection);
            }
        }
	}
}

EnemyManager::EnemyManager(entt::registry& registry, const Camera& camera, sf::Clock& gameClock)
    : registry(registry), gameClock(gameClock), camera(camera) {
}

Position EnemyManager::randomizeOffScreenPosition(const Position& position) const
{
    Position randomPosition = position;

    // Check if position is inside the screen bounds
    bool isInsideScreen = isPositionInScreen(position);

    if (isInsideScreen) {
        // Position is inside screen, move it outside at a random edge
        float halfWidth = camera.getSize().x * 0.5f;
        float halfHeight = camera.getSize().y * 0.5f;
        float centerX = camera.getbaseX();
        float centerY = camera.getbaseY();

        // Choose random edge: 0=top, 1=right, 2=bottom, 3=left
        int edge = static_cast<int>(Random::getFloat(0.0f, 4.0f));
        float spawnOffset = 100.0f + Random::getFloat(0.0f, 200.0f); // Distance beyond screen edge

        switch (edge) {
        case 0: // Top edge
            randomPosition.x = centerX + Random::getFloat(-halfWidth, halfWidth);
            randomPosition.y = centerY - halfHeight - spawnOffset;
            break;
        case 1: // Right edge
            randomPosition.x = centerX + halfWidth + spawnOffset;
            randomPosition.y = centerY + Random::getFloat(-halfHeight, halfHeight);
            break;
        case 2: // Bottom edge
            randomPosition.x = centerX + Random::getFloat(-halfWidth, halfWidth);
            randomPosition.y = centerY + halfHeight + spawnOffset;
            break;
        case 3: // Left edge
            randomPosition.x = centerX - halfWidth - spawnOffset;
            randomPosition.y = centerY + Random::getFloat(-halfHeight, halfHeight);
            break;
        }

        return randomPosition;
    }

    // Position is already outside screen, apply original logic to move it further out
    float halfWidth = camera.getSize().x * 0.5f;
    float halfHeight = camera.getSize().y * 0.5f;
    float centerX = camera.getbaseX();
    float centerY = camera.getbaseY();

    if (position.x < centerX - halfWidth) {
        randomPosition.x = position.x - 100.0f - Random::getFloat(0.0f, 200.0f);
    }
    else if (position.x > centerX + halfWidth) {
        randomPosition.x = position.x + 100.0f + Random::getFloat(0.0f, 200.0f);
    }

    if (position.y < centerY - halfHeight) {
        randomPosition.y = position.y - 100.0f - Random::getFloat(0.0f, 200.0f);
    }
    else if (position.y > centerY + halfHeight) {
        randomPosition.y = position.y + 100.0f + Random::getFloat(0.0f, 200.0f);
    }

    return randomPosition;
}

bool EnemyManager::isPositionInScreen(const Position& position) const
{
    float halfWidth = camera.getSize().x * 0.5f;
    float halfHeight = camera.getSize().y * 0.5f;
    float centerX = camera.getbaseX();
    float centerY = camera.getbaseY();

    return (position.x >= centerX - halfWidth && position.x <= centerX + halfWidth &&
        position.y >= centerY - halfHeight && position.y <= centerY + halfHeight);
}

void EnemyManager::sinkEvents()
{

}

int EnemyManager::getBaseExp(EnemyType type)
{
    const EnemyData& data = enemyLibrary.getEnemyData(type);
    return data.exp;
}

int EnemyManager::getBaseGold(EnemyType type)
{
    const EnemyData& data = enemyLibrary.getEnemyData(type);
    return data.gold;
}

Mana EnemyManager::getBaseMana(EnemyType type)
{
    const EnemyData& data = enemyLibrary.getEnemyData(type);
    return data.mana;
}