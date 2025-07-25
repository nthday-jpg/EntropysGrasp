#pragma once
#include "../components/CollisionType.h"
#include <entt/entity/entity.hpp>

struct CollisionEvent
{
    entt::entity entity1;
    entt::entity entity2;
    CollisionType type1;
    CollisionType type2;

    CollisionEvent(entt::entity e1, entt::entity e2, CollisionType t1, CollisionType t2)
        : entity1(e1), entity2(e2), type1(t1), type2(t2)
    {
        if (t1 > t2) {
            std::swap(entity1, entity2);
            std::swap(type1, type2);
		}
    }
};

struct SpellReduction {
    entt::entity spell;
};
