#pragma once
#include "SFML/System/Vector2.hpp"

// This file defines movement-related components for entities in a game.
// These components include Position, MovementDirection, and Velocity,
// which are used to manage the position, direction of movement, and speed of entities.
// All components inherit from sf::Vector2f to utilize SFML's vector functionality.

// The Position component represents the position of an entity in the game world.
struct Position : public sf::Vector2f 
{
    using sf::Vector2f::Vector2f;
};

// MovementDirection represents the direction in which an entity is moving.
struct MovementDirection : public sf::Vector2f
{
	using sf::Vector2f::Vector2f;
};

// Velocity represents the speed and direction of an entity's movement.
struct Velocity : public sf::Vector2f
{
	using sf::Vector2f::Vector2f;
};