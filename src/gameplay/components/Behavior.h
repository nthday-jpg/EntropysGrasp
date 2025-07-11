#pragma once
#include <entt/entt.hpp>

enum class BehaviorType 
{
    Straight,
    HomingEnemy,
    Orbit,
	HomingPlayer,
};

inline BehaviorType stringToBehaviorType(const std::string& str) 
{
    if (str == "Straight") return BehaviorType::Straight;
    if (str == "HomingEnemy") return BehaviorType::HomingEnemy;
    if (str == "Orbit") return BehaviorType::Orbit;
    if (str == "HomingPlayer") return BehaviorType::HomingPlayer;
    throw std::runtime_error("Unknown BehaviorType: " + str);
}