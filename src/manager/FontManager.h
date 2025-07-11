#pragma once
#include <SFML/Graphics/Font.hpp>
#include <unordered_map>

class FontManager
{
	std::unordered_map<std::string, sf::Font> fonts;
	std::string fontsPath = "assets/fonts"; 

	FontManager();
	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;

public:
	static FontManager& getInstance()
	{
		static FontManager instance;
		return instance;
	}

	sf::Font* getFont(std::string fontName);
};