#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class TextureManager {
public:
	static TextureManager& getInstance();
	std::unordered_map<std::string, sf::Texture> textureDatabase;
	bool loadTextures(const std::string& textureFilePath);
	void setTexture(const std::string& name, const sf::Texture& texture);
	const sf::Texture& getTexture(const std::string& name) const;
private:
	TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};