#include "TextureManager.h"
#include <fstream>

TextureManager& TextureManager::getInstance() {
	static TextureManager instance;
	return instance;
}

bool TextureManager::loadTextures(const std::string& textureFilePath) {
	//std::ifstream file(textureFilePath);
	//if (!file.is_open()) {
	//	return false;
	//}
	//std::string name;
	//std::string filePath;
	//while (file >> name >> filePath) {
	//	sf::Texture texture;
	//	if (!texture.loadFromFile(filePath)) {
	//		return false; // Failed to load texture
	//	}
	//	textureDatabase[name] = std::move(texture);
	//}
	//file.close();
	return true;
}

void TextureManager::setTexture(const std::string& name, const sf::Texture& texture) {
	textureDatabase[name] = texture;
}

const sf::Texture& TextureManager::getTexture(const std::string& name) const {
	auto it = textureDatabase.find(name);
	if (it != textureDatabase.end()) {
		return it->second;
	}
	throw std::runtime_error("Texture not found: " + name);
}