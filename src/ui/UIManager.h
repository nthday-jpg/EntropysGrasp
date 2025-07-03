#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

class Button;

class UIManager
{
	sf::Sprite* background;
	sf::Texture* backgroundTexture;

	std::vector<Button*> buttons;
	std::vector<sf::Text*> texts;

public:
	UIManager() = default;
	~UIManager();
	void setBackground(sf::Texture* texture);
	void draw(sf::RenderTarget& target) const;
	void handleEvent(const sf::Event& event);
	void update();
	void addButton(Button* button);
	void addText(sf::Text* text);
};