#include <SFML/System/Time.hpp>
#include <random>           
#include "../systems/EnemyManager.h"
#include "../components/EntityTags.h"
#include "../components/Enemy.h"
#include "../components/MovementComponents.h"

entt::entity EnemyManager::spawnEnemy(EnemyType type, Position position)
{
    const EnemyData& data = enemyLibrary.getEnemyData(type);

    float health = data.health.max * (1.0f + gameClock.getElapsedTime().asSeconds() * 0.01f);
    float speed = data.speed.value * (1.0f + gameClock.getElapsedTime().asSeconds() * 0.005f);
    float attack = data.attack.value * (1.0f + gameClock.getElapsedTime().asSeconds() * 0.005f);

    auto view = registry.view<InactiveEnemyTag>();
    for (auto entity : view)
    {
        registry.remove<InactiveEnemyTag>(entity);
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
    auto view = registry.view<EnemyTag>();
    for (auto entity : view) {
        auto health = registry.try_get<Health>(entity);
        // Use for some entities that may not have health
        if (!health)
        {
            continue;
        }
        if (health->current <= 0 || !isInLoadChunk(registry.get<Position>(entity)))
        {
            if (!registry.all_of<InactiveEnemyTag>(entity)) {
                registry.emplace<InactiveEnemyTag>(entity);
            }
            // Reset position to a far away place
            registry.replace<Position>(entity, -1000.0f, -1000.0f);
        }
    }
}

bool EnemyManager::isInLoadChunk(const Position& position) const
{
    if (position.x < view.getCenter().x - view.getSize().x ||
        position.x > view.getCenter().x + view.getSize().x ||
        position.y < view.getCenter().y - view.getSize().y ||
        position.y > view.getCenter().y + view.getSize().y)
    {
        return false;
    }

    return true;
}

void EnemyManager::update(float dt)
{

    spawning(dt);

    removing();
}

EnemyManager::EnemyManager(entt::registry& registry, sf::View& view, sf::Clock& gameClock)
    : registry(registry), view(view), gameClock(gameClock) {};

Position EnemyManager::randomizeOffScreenPosition(const Position& position) const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 199);
    static std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);

    Position randomPosition = position;

    // If position is inside the view/load chunk, spawn outside the screen at a random angle
    if (isInLoadChunk(position)) {
        float angle = angleDist(gen);
        float offsetX = std::cos(angle) * (view.getSize().x * 0.55f); // Slightly beyond screen edge
        float offsetY = std::sin(angle) * (view.getSize().y * 0.55f);

        // Position relative to screen center
        randomPosition.x = view.getCenter().x + offsetX;
        randomPosition.y = view.getCenter().y + offsetY;

        // Add some randomness to prevent enemies from spawning in a perfect circle
        randomPosition.x += dist(gen) * 0.5f - 50.0f;
        randomPosition.y += dist(gen) * 0.5f - 50.0f;

        return randomPosition;
    }

    // Original logic for positions already outside the view
    if (position.x < view.getCenter().x - view.getSize().x)
    {
        randomPosition.x = position.x - 100.0f - static_cast<float>(dist(gen));
    }
    else if (position.x > view.getCenter().x + view.getSize().x)
    {
        randomPosition.x = position.x + 100.0f + static_cast<float>(dist(gen));
    }
    if (position.y < view.getCenter().y - view.getSize().y)
    {
        randomPosition.y = position.y - 100.0f - static_cast<float>(dist(gen));
    }
    else if (position.y > view.getCenter().y + view.getSize().y)
    {
        randomPosition.y = position.y + 100.0f + static_cast<float>(dist(gen));
    }

    return randomPosition;
}