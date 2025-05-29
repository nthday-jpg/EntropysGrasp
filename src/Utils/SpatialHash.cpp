#include "SpatialHash.h"
#include "../components/hitbox.h"
#include "../components/movementComponents.h"

void SpatialHashGrid::clear() 
{
    grid.clear();
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
    clear();

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