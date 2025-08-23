#pragma once
#include <vector>
#include <optional>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include "UIElement.h"
#include "UIOrigin.h"

class Panel : public UIElement
{
private:
    sf::RectangleShape background;
	bool visible = true; 
    std::vector<UIElement*> children;
	sf::Vector2f position;
    bool draggable;
    bool isDragging;
    sf::Vector2f dragOffset;
    UIOrigin panelOrigin = UIOrigin::Center; // Use unified origin

public:
    Panel(sf::Vector2f position, sf::Vector2f size);
    ~Panel();

    void draw
    (
        sf::RenderTarget& target,
        sf::RenderStates states = sf::RenderStates::Default
    ) const override;

	void setDrawPosition(sf::Vector2f drawPos) override;
    bool contains(sf::Vector2i point) const override;
	void setVisible(bool visible) override;
    bool isVisible() const override;
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
    
    // Unified origin control methods
    void setOrigin(UIOrigin origin);
    void setOrigin(sf::Vector2f origin);
    UIOrigin getOrigin() const;
    sf::Vector2f getOriginPoint() const;
    
private:
    void updatePanelOrigin();
};
