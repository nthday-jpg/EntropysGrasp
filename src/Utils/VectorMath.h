#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

float magnitude(const sf::Vector2f& v);

void normalize(sf::Vector2f& v);

sf::Vector2f tangent(const sf::Vector2f& v);