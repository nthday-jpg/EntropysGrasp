#pragma once
#include "UIELement.h"
#include "UIOrigin.h"
#include "../control/GameCommandManager.h"
#include "../control/GameCommand.h"
#include "../control/commands/gameControl.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>
#include <unordered_map>

inline std::unordered_map<std::string, std::function<GameCommand* ()>> commandFactories
{
	{"EXIT", []() { return new Exit(); }},
	{"PAUSE", []() { return new Pause(); }},
	{"RESUME", []() { return new Resume(); }},
	{"RESTART", []() { return new Restart(); }},
	{"MAINMENU", []() { return new ChangeScene("MainMenu"); }},
	{"GAMEPLAY", []() { return new ChangeScene("Gameplay"); }},
	{"SHUTDOWN", []() { return new Shutdown(); } }
};

class Button : public UIElement
{
	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text text;
	sf::Vector2f position;
	bool visible = true;
	bool enabled = true;
	bool pressed = false;
	UIOrigin buttonOrigin = UIOrigin::Center; // Use unified origin
	std::function<GameCommand* ()> commandFactory;

public:
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

	void setDrawPosition(sf::Vector2f drawPos) override;
	void setVisible(bool visible) override;
	bool isVisible() const override;
	void setBackgroundColor(sf::Color color);
	void setBackground(sf::Texture* texture);
	void setTextColor(sf::Color color);
	void setPosition(sf::Vector2f position) override;
	sf::Vector2f getPosition() const override;
	void setSize(sf::Vector2f size);
	sf::Vector2f getSize() const override;
	bool contains(sf::Vector2i point) const override;
	bool handleEvent(const std::optional<sf::Event>& event) override;
	
	// Unified origin control methods
	void setOrigin(UIOrigin origin);
	void setOrigin(sf::Vector2f origin);
	UIOrigin getOrigin() const;
	sf::Vector2f getOriginPoint() const;

private:
	void updateButtonOrigin();
};