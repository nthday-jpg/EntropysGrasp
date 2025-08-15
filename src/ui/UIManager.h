#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <optional>

class UIElement;

class UIManager
{
	sf::Sprite* background;
	sf::Texture* backgroundTexture;
	sf::Font* font;

	std::vector<UIElement*> elements;

public:
	UIManager();
	~UIManager();

	void setBackground(sf::Texture* texture);

	void load();

	void clear();

	void draw(sf::RenderTarget& target) const;

	void addElement(UIElement* element);

	void syncUIWithViewport();

	bool handleEvent(const std::optional<sf::Event>& event);
};