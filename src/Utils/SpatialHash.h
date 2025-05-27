#pragma once
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include <entt/entt.hpp>

struct HashFunc {
    size_t operator()(const sf::Vector2i& v) const {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};

class SpatialHashGrid {
    float cellSize;
    std::unordered_map<sf::Vector2i, std::vector<entt::entity>, HashFunc> grid;

    void insert(entt::entity entity, const sf::Vector2f& position);
    void makeGrid(entt::registry& registry, SpatialHashGrid& spatialHashGrid);
    void clear(); // clear grid every frame
    std::vector<entt::entity> queryNearby(const sf::Vector2f& position);
public:

    //construct a function do everything
};
