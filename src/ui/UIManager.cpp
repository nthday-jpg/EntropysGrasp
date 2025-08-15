#include "UIManager.h"
#include "Button.h"

#include "../manager/WindowManager.h"

UIManager::UIManager() : background(nullptr), backgroundTexture(nullptr)
{
}

UIManager::~UIManager()
{
	clear();
}

void UIManager::load()
{
}

void UIManager::clear()
{
	for (auto& element : elements)
	{
		delete element; 
	}
	elements.clear();
	if (background)
	{
		delete background;
		background = nullptr;
		backgroundTexture = nullptr;
	}
}

void UIManager::setBackground(sf::Texture* texture)
{
	this->backgroundTexture = texture;
	this->background = new sf::Sprite(*texture);
}

void UIManager::syncUIWithViewport()
{
	sf::View view = WindowManager::getInstance().getWindow().getView();
	sf::Vector2f viewPosition = view.getCenter() - view.getSize() / 2.f;

	for(auto& element : elements)
	{
		element->setDrawPosition(viewPosition + element->getPosition());
	}
}

void UIManager::draw(sf::RenderTarget& target) const
{
	sf::RenderStates states = sf::RenderStates::Default;
	if (background)
	{
		target.draw(*background, states);
	}
	for (auto& element : elements)
	{
		if (element->isVisible())
		{
			target.draw(*element, states);
		}
	}
}

void UIManager::addElement(UIElement* element)
{
	if (element)
	{
		elements.push_back(element);
	}
	else
	{
		throw std::runtime_error("Attempted to add a null UIElement to UIManager.");
	}
}

bool UIManager::handleEvent(const std::optional<sf::Event>& event)
{
	for (auto& element : elements)
	{
		if (element->handleEvent(event))
		{
			return true; // Event was handled by an element
		}
	}
	return false;
}