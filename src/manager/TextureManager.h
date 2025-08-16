#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class TextureManager {
	std::string filePath = "assets/textures/allTexture.txt";
public:
	std::unordered_map<std::string, sf::Texture*> textureDatabase;
	
	static TextureManager& getInstance();
	bool loadFromAssetFile();
	bool loadTexture(const std::string& name, const std::string& filePath);
	void setTexture(const std::string& name, sf::Texture* texture);
	sf::Texture* getTexture(const std::string& name) const;
	TextureManager();
private:
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	~TextureManager();
};