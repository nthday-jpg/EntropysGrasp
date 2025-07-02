#include "Button.h"
#include "../control/commands/gameControl.h"
#include <SFML/Graphics/RenderTarget.hpp> 
#include "../systems/WindowManager.h"
#include <exception>


Button::Button(
	std::string command,
	sf::Font* font,
	std::string text,
	sf::Vector2f position,
	sf::Vector2f size
):	font(font),
	text(*font)
{
	//need to reposition the text
	this->text.setString(text);
	this->text.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f)); // Adjust text size based on button height
	this->text.setPosition(position);
	this->text.setFillColor(sf::Color::Black);
	this->shape.setSize(size);
	this->shape.setPosition(position);
	this->shape.setFillColor(sf::Color::White);


	if (commandFactories.find(command) != commandFactories.end())
	{
		this->commandFactory = commandFactories[command];
	}
	else
	{
		throw std::runtime_error("Command not found: " + command);
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->shape, states);
	target.draw(this->text, states);
	
}

void Button::setPosition(sf::Vector2f position)
{
	this->shape.setPosition(position);
	this->text.setPosition(position);
}

sf::Vector2f Button::getPosition() const
{
	return this->shape.getPosition();
}

void Button::setSize(sf::Vector2f size)
{
	this->shape.setSize(size);
	this->text.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f)); // Adjust text size based on button height
}

void Button::setVisible(bool visible) 
{
	this->visible = visible;
}

bool Button::isVisible() const
{
	return this->visible;
}

bool Button::contains(sf::Vector2i point) const
{
	sf::Vector2f checkPoint = WindowManager::getInstance().mapPixelToCoords(point);
	if(shape.getGlobalBounds().contains(checkPoint)) {
		return true;
	}
	return false;
}

sf::Vector2f Button::getSize() const
{
	return this->shape.getSize();
}

bool Button::handleEvent(const std::optional<sf::Event>& event)
{
	if (const auto mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button == sf::Mouse::Button::Left
			&& this->contains({ mousePressed->position.x, mousePressed->position.y}))
		{
			UICommand* command = commandFactory();
			if (command)
			{
				UICommandManager::getInstance().queueCommand(command);
				return true; 
			}
			return false;
		}
	}
	return true;
}