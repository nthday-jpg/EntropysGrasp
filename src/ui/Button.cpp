#include "Button.h"
#include "../control/commands/gameControl.h"
#include <SFML/Graphics/RenderTarget.hpp> 
#include "../systems/WindowManager.h"
#include <exception>


Button::Button(
	std::string command,
	std::string text,
	sf::Font* font,
	sf::Vector2f position,
	int characterSize
):	font(font),
	text(*font),
	position(position)
{
	
	sf::View view = WindowManager::getInstance().getWindow().getView();
	sf::Vector2f viewPosition = view.getCenter() - view.getSize() / 2.f;
	sf::Vector2f drawPos = position + viewPosition;

	this->text.setString(text);
	this->text.setFillColor(sf::Color::Red);
	this->text.setCharacterSize(characterSize);
	this->text.setPosition(drawPos);

	sf::FloatRect textBounds = this->text.getLocalBounds();
	this->shape.setSize(textBounds.size);
	this->shape.setPosition(drawPos + textBounds.position);
	this->shape.setFillColor(sf::Color::Transparent);

	if (commandFactories.find(command) != commandFactories.end())
	{
		this->commandFactory = commandFactories[command];
	}
	else
	{
		throw std::runtime_error("Command not found: " + command);
	}
}

void Button::setDrawPosition(sf::Vector2f drawPos)
{
	this->text.setPosition(drawPos);
	this->shape.setPosition(drawPos + this->text.getLocalBounds().position);
}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!this->visible) return;
	target.draw(this->shape, states);
	target.draw(this->text, states);
}

void Button::setPosition(sf::Vector2f position)
{
	this->position = position;
}

sf::Vector2f Button::getPosition() const
{
	return this->position;
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
	sf::Vector2f worldPoint = WindowManager::getInstance().mapPixelToCoords(point);
	if(shape.getGlobalBounds().contains(worldPoint)) {
		return true;
	}
	return false;
}

sf::Vector2f Button::getSize() const
{
	return this->shape.getSize();
}

void Button::setTextColor(sf::Color color)
{
	this->text.setFillColor(color);
}

void Button::setBackgroundColor(sf::Color color)
{
	this->shape.setFillColor(color);
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
		}
	}
	return false;
}