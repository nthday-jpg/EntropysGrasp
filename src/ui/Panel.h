#pragma once
#include <vector>
#include <optional>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include "UIElement.h"

enum class PanelOrigin {
	TopLeft,
	TopCenter,
	TopRight,
	CenterLeft,
	Center,
	CenterRight,
	BottomLeft,
	BottomCenter,
	BottomRight
};

class Panel : public UIElement
{
private:
    sf::RectangleShape background;
	bool visible = true; 
	// List of child UI elements
	// The position of each child is relative to the panel's position
    std::vector<UIElement*> children;

	// Position of the panel in window coordinates
	sf::Vector2f position;
    bool draggable;
    bool isDragging;
    sf::Vector2f dragOffset;
    
    PanelOrigin panelOrigin = PanelOrigin::Center; // Default to center like other UI elements

public:
    Panel(sf::Vector2f position, sf::Vector2f size);
    ~Panel();

    // UIElement interface implementation
    void draw
    (
        sf::RenderTarget& target,
        sf::RenderStates states = sf::RenderStates::Default
    ) const override;

	void setDrawPosition(sf::Vector2f drawPos) override;

    bool contains(sf::Vector2i point) const override;
   
	// The panel does not handle scroll events, so this method is empty
	void setVisible(bool visible) override;
    bool isVisible() const override;

	//handle the scroll is not implemented yet
    bool handleEvent(const std::optional<sf::Event>& event) override;

    void setPosition(sf::Vector2f position) override;

    sf::Vector2f getPosition() const override;

    sf::FloatRect getGlobalBounds() const;

	sf::Vector2f getSize() const override;

    // Panel-specific methods
    void addElement(UIElement* element);
    void setBackgroundColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    void setOutlineThickness(float thickness);
    void setDraggable(bool draggable);
    
    // Origin control methods
    void setOrigin(PanelOrigin origin);
    void setOrigin(sf::Vector2f origin);
    PanelOrigin getOrigin() const;
    sf::Vector2f getOriginPoint() const;
    
private:
    void updatePanelOrigin();
};
