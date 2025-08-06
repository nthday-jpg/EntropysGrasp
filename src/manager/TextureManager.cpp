#include "TextureManager.h"
#include <fstream>
#include <sstream>
#include <cassert>

TextureManager::TextureManager() {
    if (!loadFromAssetFile()) {
        assert(false && "Failed to load textures from asset file");
	}
}

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

bool TextureManager::loadTexture(const std::string& name, const std::string& filePath) {
    if (textureDatabase.count(name) > 0)
        return true;

    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(filePath)) {
        delete texture;
        assert(false && "Failed to load texture");
        return false;
    }

    textureDatabase[name] = texture;
    return true;
}

void TextureManager::setTexture(const std::string& name, sf::Texture* texture) {
    if (textureDatabase.count(name) > 0) {
        delete textureDatabase[name];
    }
    textureDatabase[name] = texture;
}

sf::Texture* TextureManager::getTexture(const std::string& name) const {
    auto it = textureDatabase.find(name);
    if (it != textureDatabase.end())
        return it->second;
    return nullptr;
}

bool TextureManager::loadFromAssetFile() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        assert(false && "Failed to open asset file");
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string name, path;

        if (!(ss >> name >> path)) {
            continue; 
        }

        if (!loadTexture(name, path)) {
            assert(false && "Failed to load one or more textures from asset file");
            return false;
        }
    }

    return true;
}

TextureManager::~TextureManager() {
    for (auto& pair : textureDatabase) {
        delete pair.second;
    }
    textureDatabase.clear();
}