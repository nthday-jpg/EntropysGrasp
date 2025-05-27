#pragma once
#include "SFML/System/Vector2.hpp"

struct Position : public sf::Vector2f
{
};

//MovementDirection component dùng để quản lý hướng di chuyển của người chơi
//Hướng di chuyển được xác định bằng các phím WASD hoặc các phím mũi tên
struct MovementDirection : sf::Vector2f
{
};