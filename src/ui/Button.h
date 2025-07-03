#pragma once
#include "UIELement.h"
#include "../control/UICommandManager.h"
#include "../control/UICommand.h"
#include "../control/commands/gameControl.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>
#include <unordered_map>

inline std::unordered_map<std::string, std::function<UICommand* ()>> commandFactories
{
	{"exit", []() { return new Exit(&UICommandManager::getInstance()); }},
	{"pause", []() { return new Pause(&UICommandManager::getInstance()); }},
	{"resume", []() { return new Resume(&UICommandManager::getInstance()); }},
	{"restart", []() { return new Restart(&UICommandManager::getInstance()); }}
};

class Button : public UIElement
{
	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text text;

	// Position of the button in the window
	sf::Vector2f position;

	bool visible = true;
	bool enabled = true;
	bool pressed = false;

	std::function<UICommand* ()> commandFactory;

public:
	//No background by default
	Button(
		std::string command,
		std::string text,
		sf::Font* font,
		sf::Vector2f position = { 0.f, 0.f },
		int characterSize = 30
	);

	void draw
	(
		sf::RenderTarget& target,
		sf::RenderStates states = sf::RenderStates::Default
	) const override;

	// update the position button and text in world coordinates
	void update(sf::Vector2f drawPos);

	void setVisible(bool visible) override;

	bool isVisible() const override;

	void setBackgroundColor(sf::Color color);

	void setTextColor(sf::Color color);

	// Set the position of the button relative to the window
	void setPosition(sf::Vector2f position) override;

	// Get the position of the button relative to the window
	sf::Vector2f getPosition() const override;

	void setSize(sf::Vector2f size) override;

	sf::Vector2f getSize() const override;

	bool contains(sf::Vector2i point) const override;

	bool handleEvent(const std::optional<sf::Event>& event) override;
};