#include "UIManager.h"
#include "Button.h"

#include "../systems/WindowManager.h"


UIManager::~UIManager()
{
	for (auto& button : buttons)
	{
		delete button;
	}
	buttons.clear();

	for (auto& text : texts)
	{
		delete text;
	}
	texts.clear();
}

void UIManager::setBackground(sf::Texture* texture)
{
	this->backgroundTexture = texture;
	this->background = new sf::Sprite(*texture);
}

void UIManager::update()
{
	sf::View view = WindowManager::getInstance().getWindow().getView();
	sf::Vector2f viewPosition = view.getCenter() - view.getSize() / 2.f;

	for (auto& button : buttons)
	{
		sf::Vector2f drawPos = button->getPosition() + viewPosition;
		button->update(drawPos);
	}

	for (auto& text : texts)
	{
		sf::Vector2f drawPos = text->getPosition() + viewPosition;
		text->setPosition(drawPos);
	}
}

void UIManager::draw(sf::RenderTarget& target) const
{
	sf::RenderStates states = sf::RenderStates::Default;
	if (background)
	{
		target.draw(*background, states);
	}
	for (const auto& button : buttons)
	{
		target.draw(*button, states);
	}
	for (const auto& text : texts)
	{
		target.draw(*text, states);
	}
}

void UIManager::addButton(Button* button)
{
	buttons.push_back(button);
}

void UIManager::addText(sf::Text* text)
{
	texts.push_back(text);
}

bool UIManager::handleEvent(const sf::Event& event)
{
	for (auto& button : buttons)
	{
		if (button->handleEvent(event) == true)
		{
			return true;
		}
	}
	return false;
}