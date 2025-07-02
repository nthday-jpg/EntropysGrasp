#pragma once
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

class Button;
class Text;

class UIManager
{
	sf::Sprite* background;
	sf::Texture* backgroundTexture;

	std::vector<Button> buttons;
	std::vector<Text> texts;

};