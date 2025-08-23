#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>


class UIElement : public sf::Drawable {

public:
    virtual ~UIElement() = default;

	// Set center position in window coordinates
    virtual void setPosition(sf::Vector2f centerPosition) = 0;

	// Get center position of the element in window coordinates
    virtual sf::Vector2f getPosition() const = 0;

	// set the center position of the element in world coordinates for rendering
	virtual void setDrawPosition(sf::Vector2f centerDrawPos) = 0;

    // Check if point is inside element
	// The point is in window coordinates
    virtual bool contains(sf::Vector2i point) const = 0;

    // Handle event (returns true if event was handled)
    virtual bool handleEvent(const std::optional<sf::Event>& event) = 0;

	virtual void setVisible(bool visible) = 0;

	virtual bool isVisible() const = 0;

	// Get the size of the element (needed for center positioning calculations)
	virtual sf::Vector2f getSize() const = 0;

	//draw is pure virtual inherited from sf::Drawable
};
