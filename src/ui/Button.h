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

	bool visible = true;
	bool enabled = true;
	bool pressed = false;

	std::function<UICommand* ()> commandFactory;

public:

	Button(
		std::string command,
		sf::Font* font,
		std::string text,
		sf::Vector2f position = { 0.f, 0.f },
		sf::Vector2f size = { 100.f, 30.f }
	);

	void draw
	(
		sf::RenderTarget& target,
		sf::RenderStates states = sf::RenderStates::Default
	) const override;

	void setVisible(bool visible) override;

	bool isVisible() const override;

	void setPosition(sf::Vector2f position) override;

	sf::Vector2f getPosition() const override;

	void setSize(sf::Vector2f size) override;

	sf::Vector2f getSize() const override;

	bool contains(sf::Vector2i point) const override;

	bool handleEvent(const std::optional<sf::Event>& event) override;
};