#pragma once
#include"UIELement.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>

enum class TextOrigin {
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

class Text : public UIElement {
	sf::Text text;

	sf::Vector2f position;

	bool visible;
	
	TextOrigin textOrigin = TextOrigin::Center; // Default to center

public:
	Text(const sf::Font& font, std::string text, sf::Vector2f position, unsigned int charSize = 30);

	void draw
	(
		sf::RenderTarget& target, 
		sf::RenderStates states = sf::RenderStates::Default
	) const override;

	void setString(std::string text);

	void setPosition(sf::Vector2f position) override;

	sf::Vector2f getPosition() const override;

	void setDrawPosition(sf::Vector2f drawPos) override;

	bool contains(sf::Vector2i point) const override;

	bool handleEvent(const std::optional<sf::Event>& event) override;

	void setVisible(bool visible) override;

	bool isVisible() const override;

	sf::FloatRect getGlobalBounds() const;

	void setCharacterSize(unsigned int charSize);

	void setFillColor(sf::Color color);

	sf::Vector2f getSize() const override;

	void updateTextOrigin();
	
	// New methods for origin control
	void setOrigin(TextOrigin origin);
	void setOrigin(sf::Vector2f origin);
	TextOrigin getOrigin() const;
	sf::Vector2f getOriginPoint() const;
};