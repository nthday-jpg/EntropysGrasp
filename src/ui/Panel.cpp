#include "Panel.h"
#include "../manager/WindowManager.h"
#include <SFML/Graphics/View.hpp>

Panel::Panel(sf::Vector2f position, sf::Vector2f size)
	: position(position), draggable(false), isDragging(false), dragOffset(0.f, 0.f), panelOrigin(UIOrigin::Center)
{
	background.setSize(size);
	background.setFillColor(sf::Color(50, 50, 50, 200));
	background.setOutlineColor(sf::Color::Black);
	background.setOutlineThickness(1.f);
	
	updatePanelOrigin();
}

Panel::~Panel()
{
	for (auto& child : children)
	{
		delete child; // Clean up child elements
	}
	children.clear();
}

void Panel::updatePanelOrigin()
{
	sf::Vector2f size = background.getSize();
	sf::Vector2f origin = UIOriginHelper::calculateOrigin(size, panelOrigin);
	background.setOrigin(origin);
}

void Panel::setOrigin(UIOrigin origin)
{
	this->panelOrigin = origin;
	updatePanelOrigin();
}

void Panel::setOrigin(sf::Vector2f origin)
{
	background.setOrigin(origin);
}

UIOrigin Panel::getOrigin() const
{
	return panelOrigin;
}

sf::Vector2f Panel::getOriginPoint() const
{
	return background.getOrigin();
}

void Panel::setPosition(sf::Vector2f position)
{
	this->position = position;
}

void Panel::setDrawPosition(sf::Vector2f drawPos)
{
	background.setPosition(drawPos);
	for (auto& child : children)
	{
		child->setDrawPosition(drawPos + child->getPosition());
	}
}

void Panel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!visible) return;
	
	target.draw(background, states);
	for (const auto& child : children)
	{
		if (child->isVisible())
		{
			target.draw(*child, states);
		}
	}
}

bool Panel::handleEvent(const std::optional<sf::Event>&event)
{
	if (!visible) return false; 
	if (!event.has_value()) return false; // No event to handle

	if (const auto& mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
	{
		if(mousePressed->button == sf::Mouse::Button::Left)
		{
			if (!this->contains(mousePressed->position))
			{
				return false; // Click was outside the panel
			}

			for (auto& child : children)
			{
				if (child->handleEvent(event))
				{
					return true; // Child handled the event
				}
			}

			isDragging = draggable;
			dragOffset = static_cast<sf::Vector2f>(mousePressed->position) - this->position;
			return true; // Panel is being dragged
		}
	}
	else if (const auto& mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
	{
		if (mouseReleased->button == sf::Mouse::Button::Left)
		{
			isDragging = false; // Stop dragging
			return true; // Event handled
		}
	}
	else if (const auto& mouseMoved = event->getIf<sf::Event::MouseMoved>())
	{
		if (isDragging)
		{
			sf::Vector2f newPos = static_cast<sf::Vector2f>(mouseMoved->position) - dragOffset;
			setPosition(newPos);
			return true; // Panel is being dragged
		}
	}
	return false;
}

sf::FloatRect Panel::getGlobalBounds() const
{
	return background.getGlobalBounds();
}

sf::Vector2f Panel::getPosition() const
{
	return this->position;
}

void Panel::addElement(UIElement* element)
{
	if (element)
	{
		children.push_back(element);
		// Children are positioned relative to panel center
		sf::Vector2f childWorldPos = this->position + element->getPosition();
		element->setDrawPosition(childWorldPos);
	}
	else
	{
		throw std::runtime_error("Attempted to add a null UIElement to Panel.");
	}
}

void Panel::setBackgroundColor(sf::Color color)
{
	background.setFillColor(color);
}

void Panel::setOutlineColor(sf::Color color)
{
	background.setOutlineColor(color);
}

void Panel::setOutlineThickness(float thickness)
{
	background.setOutlineThickness(thickness);
}

void Panel::setDraggable(bool draggable)
{
	this->draggable = draggable;
}

bool Panel::contains(sf::Vector2i point) const
{
	sf::Vector2f worldPoint = WindowManager::getInstance().mapPixelToCoords(point);
	return background.getGlobalBounds().contains(worldPoint);
}

bool Panel::isVisible() const
{
	return visible;
}

void Panel::setVisible(bool visible)
{
	this->visible = visible; 
}

sf::Vector2f Panel::getSize() const
{
	return background.getSize();
}

