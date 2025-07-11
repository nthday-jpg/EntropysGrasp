#include "FontManager.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

FontManager::FontManager() 
{
	for (const auto& entry : fs::directory_iterator(fontsPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".ttf") {
			sf::Font font;
			if (font.openFromFile(entry.path().string()))
			{
				std::string fontName = entry.path().stem().string();
				fonts[fontName] = std::move(font);
			}
			else
			{
				std::cerr << "Failed to load font: " << entry.path().string() << std::endl;
			}
		}
	}
	if (fonts.empty()) {
		throw std::runtime_error("No fonts found in " + fontsPath);
	}
}

sf::Font* FontManager::getFont(std::string fontName)
{
	if (fonts.find(fontName) != fonts.end()) {
		return &fonts[fontName];
	} else {
		std::cerr << "Font not found: " << fontName << std::endl;
		return nullptr; // or throw an exception
	}
}