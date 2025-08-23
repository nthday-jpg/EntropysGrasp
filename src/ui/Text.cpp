#include "Text.h"
#include "../manager/WindowManager.h"

Text::Text(
	const sf::Font& font, std::string text, 
	sf::Vector2f position, unsigned int charSize
):	text(font, text, charSize),
	position(position), visible(true), textOrigin(UIOrigin::Center)
{
	this->text.setFillColor(sf::Color::White); // Default color
	this->text.setCharacterSize(charSize);
	
	// Set text origin to center by default
	updateTextOrigin();
	
	// Set initial position
	this->text.setPosition(position);
}

void Text::updateTextOrigin()
{
	sf::FloatRect textBounds = this->text.getLocalBounds();
	sf::Vector2f textSize(textBounds.size.x, textBounds.size.y);
	sf::Vector2f offset(textBounds.position.x, textBounds.position.y);
	
	sf::Vector2f origin = UIOriginHelper::calculateOrigin(textSize, textOrigin);
	this->text.setOrigin(offset + origin);
}

void Text::setOrigin(UIOrigin origin)
{
	this->textOrigin = origin;
	updateTextOrigin();
}

void Text::setOrigin(sf::Vector2f origin)
{
	this->text.setOrigin(origin);
	// When setting custom origin, we don't update textOrigin enum
}

UIOrigin Text::getOrigin() const
{
	return textOrigin;
}

sf::Vector2f Text::getOriginPoint() const
{
	return this->text.getOrigin();
}

void Text::setString(std::string text) 
{
	this->text.setString(text);
	updateTextOrigin(); // Recalculate origin when text changes
}

void Text::setPosition(sf::Vector2f position) 
{
	this->position = position;
}

sf::Vector2f Text::getPosition() const 
{
	return this->position;
}

sf::Vector2f Text::getSize() const
{
	sf::FloatRect bounds = this->text.getGlobalBounds();
	return sf::Vector2f(bounds.size.x, bounds.size.y);
}

sf::FloatRect Text::getGlobalBounds() const 
{
	return this->text.getGlobalBounds();
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (this->visible) {
		target.draw(this->text, states);
	}
}

void Text::setFillColor(sf::Color color) {
	this->text.setFillColor(color);
}

void Text::setCharacterSize(unsigned int charSize) {
	this->text.setCharacterSize(charSize);
	updateTextOrigin(); // Recalculate origin when size changes
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
	return false;
}

void Text::setDrawPosition(sf::Vector2f centerDrawPos) {
	this->text.setPosition(centerDrawPos);
}