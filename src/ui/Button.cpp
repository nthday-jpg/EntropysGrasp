#include "Button.h"
#include "../control/commands/gameControl.h"
#include <SFML/Graphics/RenderTarget.hpp> 
#include "../manager/WindowManager.h"
#include <exception>


Button::Button(
	std::string command,
	std::string text,
	sf::Font* font,
	sf::Vector2f position,
	int characterSize
):	font(font),
	text(*font),
	position(position),
	buttonOrigin(ButtonOrigin::Center) // Initialize buttonOrigin
{
	this->text.setString(text);
	this->text.setFillColor(sf::Color::Red);
	this->text.setCharacterSize(characterSize);

	// Calculate text bounds to determine button size
	sf::FloatRect textBounds = this->text.getLocalBounds();
	sf::Vector2f buttonSize = sf::Vector2f(textBounds.size.x + 20.0f, textBounds.size.y + 10.0f); // Add padding
	this->shape.setSize(buttonSize);
	this->shape.setFillColor(sf::Color::Transparent);

	// Center the text within the button
	sf::Vector2f textOffset = sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.0f,
		textBounds.position.y + textBounds.size.y / 2.0f
	);
	this->text.setOrigin(textOffset);

	// Set button origin (must be called after setting size)
	updateButtonOrigin();

	// Set initial draw position
	setDrawPosition(position);

	if (commandFactories.find(command) != commandFactories.end())
	{
		this->commandFactory = commandFactories[command];
	}
	else
	{
		throw std::runtime_error("Command not found: " + command);
	}
}

void Button::updateButtonOrigin()
{
	sf::Vector2f size = this->shape.getSize(); // Use getSize() not getGlobalBounds()
	sf::Vector2f origin;

	switch (this->buttonOrigin)
	{
	case ButtonOrigin::TopLeft:
		origin = { 0.0f, 0.0f };
		break;
	case ButtonOrigin::TopCenter:
		origin = { size.x / 2.0f, 0.0f };
		break;
	case ButtonOrigin::TopRight:
		origin = { size.x, 0.0f };
		break;
	case ButtonOrigin::CenterLeft:
		origin = { 0.0f, size.y / 2.0f };
		break;
	case ButtonOrigin::Center:
		origin = { size.x / 2.0f, size.y / 2.0f };
		break;
	case ButtonOrigin::CenterRight:
		origin = { size.x, size.y / 2.0f };
		break;
	case ButtonOrigin::BottomLeft:
		origin = { 0.0f, size.y };
		break;
	case ButtonOrigin::BottomCenter:
		origin = { size.x / 2.0f, size.y };
		break;
	case ButtonOrigin::BottomRight:
		origin = { size.x, size.y };
		break;
	}

	this->shape.setOrigin(origin); // Set origin on the shape, not the text
}

void Button::setDrawPosition(sf::Vector2f drawPos)
{
	// Set button shape position (origin is already set on the shape)
	this->shape.setPosition(drawPos);
	
	// Set text to be centered relative to the button shape's actual position
	sf::Vector2f shapeCenter = this->shape.getPosition() + (this->shape.getSize() / 2.0f) - this->shape.getOrigin();
	this->text.setPosition(shapeCenter);
}

void Button::setOrigin(ButtonOrigin origin)
{
	this->buttonOrigin = origin;
	updateButtonOrigin();
}

void Button::setOrigin(sf::Vector2f origin)
{
	this->shape.setOrigin(origin); // Set origin on shape, not text
	// When setting custom origin, we don't update buttonOrigin enum
	// This allows for completely custom positioning
}

ButtonOrigin Button::getOrigin() const
{
	return buttonOrigin;
}

sf::Vector2f Button::getOriginPoint() const
{
	return this->shape.getOrigin(); // Return shape origin, not text origin
}

void Button::setSize(sf::Vector2f size)
{
	this->shape.setSize(size);
	this->text.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f)); // Adjust text size based on button height
	
	// Recalculate text centering when size changes
	sf::FloatRect textBounds = this->text.getLocalBounds();
	sf::Vector2f textOffset = sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.0f,
		textBounds.position.y + textBounds.size.y / 2.0f
	);
	this->text.setOrigin(textOffset);
	
	// Update button origin when size changes
	updateButtonOrigin();
	
	// Update draw positions with current position
	setDrawPosition(this->position);
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

sf::Vector2f Button::getSize() const
{
	return this->shape.getSize();
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

void Button::setTextColor(sf::Color color)
{
	this->text.setFillColor(color);
}

void Button::setBackgroundColor(sf::Color color)
{
	this->shape.setFillColor(color);
}

void Button::setBackground(sf::Texture* texture)
{
	if (texture) {
		this->shape.setTexture(texture);
	}
	else {
		this->shape.setTexture(nullptr);
	}
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