#include "SpatialHash.h"
#include "../gameplay/components/Hitbox.h"
#include "../gameplay/components/MovementComponents.h"

SpatialHashGrid::SpatialHashGrid(float cellSize) : cellSize(cellSize)
{
    grid.reserve(1000);
}

void SpatialHashGrid::insert(entt::entity entity, const sf::Vector2f& position) 
{
    int cellX = (int)(std::floor(position.x / cellSize));
    int cellY = (int)(std::floor(position.y / cellSize));
    sf::Vector2i cellPosition(cellX, cellY);
    grid[cellPosition].push_back(entity);
}

void SpatialHashGrid::makeGrid(entt::registry& registry)
{
    auto view = registry.view<Position, Hitbox>();
    for (auto [entity, pos, vel] : view.each())
    {
        insert(entity, pos);
    }
}

std::vector<entt::entity> SpatialHashGrid::queryNearby(const sf::Vector2f& position) 
{
    std::vector<entt::entity> result;
    int cellX = (int)(std::floor(position.x / cellSize));
    int cellY = (int)(std::floor(position.y / cellSize));
    for (int dx = -1; dx <= 1; ++dx) 
    {
        for (int dy = -1; dy <= 1; ++dy) 
        {
            sf::Vector2i nearCell(cellX + dx, cellY + dy);
            auto it = grid.find(nearCell);
            if (it != grid.end()) 
            {
                result.insert(result.end(), it->second.begin(), it->second.end());
            }
        }
    }
    return result;
}