#pragma once

enum class CollisionType 
{
    Player,
    Enemy,
    Spell,
    Wall,
    None
};

inline bool isSolid(const CollisionType& type1, const CollisionType& type2)
{
    return (type1 == CollisionType::Enemy && type2 == CollisionType::Player)
        || (type1 == CollisionType::Player && type2 == CollisionType::Enemy)
        || (type1 == CollisionType::Wall && type2 == CollisionType::Enemy)
        || (type1 == CollisionType::Enemy && type2 == CollisionType::Wall)
        || (type1 == CollisionType::Player && type2 == CollisionType::Wall)
        || (type1 == CollisionType::Wall && type2 == CollisionType::Player);
}