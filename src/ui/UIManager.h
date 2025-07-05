#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

class UIElement;

class UIManager
{
	sf::Sprite* background;
	sf::Texture* backgroundTexture;

	std::vector<UIElement*> elements;

public:
	UIManager() = default;
	~UIManager();

	void setBackground(sf::Texture* texture);

	void draw(sf::RenderTarget& target) const;

	void addElement(UIElement* element);

	void syncUIWithViewport();

	bool handleEvent(const sf::Event& event);
};