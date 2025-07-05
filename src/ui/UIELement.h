#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>


class UIElement : public sf::Drawable {

public:
    virtual ~UIElement() = default;

	// Set position in window coordinates
    virtual void setPosition(sf::Vector2f position) = 0;

	// Get position of the element in world coordinates
    virtual sf::Vector2f getPosition() const = 0;

	// set the position of the element in world coordinates
	virtual void setDrawPosition(sf::Vector2f drawPos) = 0;

    // Check if point is inside element
	// The point is in window coordinates
    virtual bool contains(sf::Vector2i point) const = 0;

    // Handle event (returns true if event was handled)
    virtual bool handleEvent(const std::optional<sf::Event>& event) = 0;

	virtual void setVisible(bool visible) = 0;

	virtual bool isVisible() const = 0;

	//draw is pure virtual inherited from sf::Drawable
};
