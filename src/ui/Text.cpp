#include "Text.h"
#include "../manager/WindowManager.h"

Text::Text(
	const sf::Font& font, std::string text, 
	sf::Vector2f position, unsigned int charSize
):	text(font, text, charSize),
	position(position), visible(true)
{
	// Don't calculate draw position in constructor - let syncUIWithViewport handle it
	this->text.setPosition(position); // Set initial position
	this->text.setFillColor(sf::Color::White); // Default color
	this->text.setCharacterSize(charSize);
}

void Text::setString(std::string text) 
{
	this->text.setString(text);
}

void Text::setPosition(sf::Vector2f position) 
{
	this->position = position; // Store logical position
	// Don't update SFML text position here - let setDrawPosition handle it
}

sf::Vector2f Text::getPosition() const 
{
	return this->position; // Return logical position, not SFML text position
}

sf::FloatRect Text::getGlobalBounds() const 
{
	return this->text.getGlobalBounds();
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->text, states);
}

void Text::setFillColor(sf::Color color) {
	this->text.setFillColor(color);
}

void Text::setCharacterSize(unsigned int charSize) {
	this->text.setCharacterSize(charSize);
}

void Text::setVisible(bool visible) {
	this->visible = visible;
}

bool Text::isVisible() const {
	return this->visible;
}

bool Text::contains(sf::Vector2i point) const
{
	sf::Vector2f worldPoint = WindowManager::getInstance().mapPixelToCoords(point);
	if (text.getGlobalBounds().contains(worldPoint)) {
		return true;
	}
	return false;
}

bool Text::handleEvent(const std::optional<sf::Event>& event) 
{
	// Text does not handle events, return false
	return false;
}

void Text::setDrawPosition(sf::Vector2f drawPos) {
	this->text.setPosition(drawPos); // Set actual SFML text position for rendering
}
