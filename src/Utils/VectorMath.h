#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

float magnitude(const sf::Vector2f& v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(const sf::Vector2f& v) {
	float mag = magnitude(v);
	if (mag == 0) return {0, 0}; // Avoid division by zero
	return {v.x / mag, v.y / mag};
}

sf::Vector2f tangent(const sf::Vector2f& v) {
	float mag = magnitude(v);
	if (mag == 0) return {0, 0}; // Avoid division by zero
	return {-v.y / mag, v.x / mag}; // Perpendicular vector
}

