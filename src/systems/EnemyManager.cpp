#include <SFML/System/Time.hpp>
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
		EnemyType existingType = registry.get<EnemyType>(entity);
		if (existingType != type) 
        {
			continue; 
		}

        registry.remove<InactiveEnemyTag>(entity);
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
    registry.emplace<Resistance>(entity, data.resistance);
    registry.emplace<EnemyType>(entity, type);
    return entity;
}

void EnemyManager::spawning(const std::vector<EnemySpawnInfo>& spawnInfos, float dt) 
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
                    spawnEnemy(info.type, position);
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

bool EnemyManager::isInLoadChunk(Position position)
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

	spawning(spawnInfos, dt);

	removing();
}

EnemyManager::EnemyManager(entt::registry registry, EnemyLibrary& enemyLibrary, 
                            sf::View& view, sf::Clock& gameClock)
    : registry(registry), enemyLibrary(enemyLibrary), view(view), gameClock(gameClock) 
{
};