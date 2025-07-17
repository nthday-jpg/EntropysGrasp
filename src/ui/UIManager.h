#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
#include <nlohmann/json.hpp>

class UIElement;

class UIManager
{
	sf::Font* font;
	sf::Sprite* background;
	sf::Texture* backgroundTexture;

	std::vector<UIElement*> elements;

public:
	UIManager() = default;
	~UIManager();

	UIElement* parseElement(const nlohmann::json& elemJson, sf::Font* font);

	void loadFile(const std::string& filePath);

	void setBackground(sf::Texture* texture);

	void draw(sf::RenderTarget& target) const;

	void addElement(UIElement* element);

	void syncUIWithViewport();

	bool handleEvent(const std::optional<sf::Event>& event);
};