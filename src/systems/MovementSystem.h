#pragma once
#include <entt/entt.hpp>
#include "../components/EntityTag.h"
struct PlayerMovementSystem
{
	static void update(entt::registry& registry, float dt);

	//Hàm tính toán và trả về tốc độ 
	//tuỳ vào nhều yếu tố như máu, mana, hướng nhìn, địa hình,..
	static float calculatedSpeed(entt::registry& registry, entt::entity playerEntity);
};

struct EnemyMovementSystem
{
	static void update(entt::registry& registry, float dt);
};