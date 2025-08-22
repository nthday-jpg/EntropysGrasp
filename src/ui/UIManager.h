#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include "../manager/SoundManager.h"
#include "Text.h"
#include "UIElement.h"
#include <optional>
#include <nlohmann/json.hpp>
#include <functional>

class UIManager
{
	sf::Font* font;
	sf::Sprite* background;
	sf::Texture* backgroundTexture;
	entt::dispatcher* dispatcher = nullptr;

	std::vector<UIElement*> elements;
	
	// Map to store dynamic text elements by ID for easy updates
	std::unordered_map<std::string, Text*> dynamicTexts;
	
	// Map to store update functions for dynamic texts
	std::unordered_map<std::string, std::function<std::string()>> textUpdaters;

	UIElement* parseElement(const nlohmann::json& elemJson, sf::Font* font);
public:
	UIManager();
	~UIManager();


	void loadFile(const std::string& filePath);

	void bindDispatcher(entt::dispatcher* dispatcher);

	void setBackground(sf::Texture* texture);

	void load();

	void clear();

	void draw(sf::RenderTarget& target) const;

	void addElement(UIElement* element);
	
	// Add dynamic text element with update function
	void addDynamicText(const std::string& id, Text* textElement, std::function<std::string()> updater);
	
	// Update all dynamic texts
	void updateDynamicTexts();
	
	// Update specific dynamic text
	void updateDynamicText(const std::string& id);

	void syncUIWithViewport();

	bool handleEvent(const std::optional<sf::Event>& event);
};