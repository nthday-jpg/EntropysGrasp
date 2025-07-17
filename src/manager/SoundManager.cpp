#include "SoundManager.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

void SoundManager::bindDispatcher(entt::dispatcher* dispatcher) {
	this->dispatcher = dispatcher;
	
	// Load sound buffers
	//for (const auto& [name, filePath] : soundFiles) {
	//	sf::SoundBuffer buffer;
	//	if (buffer.loadFromFile(filePath)) {
	//		buffers[name] = std::move(buffer);
	//	} else {
	//		throw std::runtime_error("Failed to load sound file: " + filePath);
	//	}
	//}
	//// Initialize sounds with their respective buffers
	//for (const auto& [name, buffer] : buffers) {
	//	sf::Sound sound;
	//	sound.setBuffer(buffer);
	//	sounds[name] = std::move(sound);
	//}
	//// Load background music
	//if (!backgroundMusic.openFromFile(soundFiles["background"])) {
	//	throw std::runtime_error("Failed to load background music");
	//}
}

void SoundManager::loadSounds(const std::string& sourcePath)
{
	std::ifstream file(sourcePath);
	if (!file) {
		std::cerr << "Error: Could not open sound file: " << sourcePath << std::endl;
		throw std::runtime_error("Failed to open sound file: " + sourcePath);
	}

	nlohmann::json soundData;
	file >> soundData;
	file.close();

	for (auto& [name, path] : soundData.items())
	{
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile(path)) {
			std::cerr << "Error: Could not load sound file: " << path << std::endl;
			throw std::runtime_error("Failed to load sound file: " + path);
		}
		soundBuffers[name] = buffer;

		sf::Sound sound(buffer);
		sounds[name] = sound;
	}
}

void SoundManager::loadMusics(const std::string& sourcePath)
{
	std::ifstream file(sourcePath);
	if (!file) {
		std::cerr << "Error: Could not open sound file: " << sourcePath << std::endl;
		throw std::runtime_error("Failed to open sound file: " + sourcePath);
	}

	nlohmann::json musicData;
	file >> musicData;
	file.close();

	for (auto& [name, path] : musicData.items())
	{
		musicFiles[name] = path;
	}
}

void SoundManager::playSound(const std::string& name, bool loop = false)
{
	auto bufferIt = soundBuffers.find(name);
	if (bufferIt == soundBuffers.end()) {
		std::cerr << "Sound buffer not found: " << name << std::endl;
		return; 
	}

	auto& sound = sounds[name]; 
	sound.setBuffer(bufferIt->second);
	sound.setLooping(loop);
	sound.setVolume(volume);
	sound.play();

}

void SoundManager::stopSound(const std::string& name)
{

}

void SoundManager::stopAllSounds()
{

}

void SoundManager::setBackgroundMusic(const std::string& filePath, bool loop = true)
{

}

void SoundManager::playBackgroundMusic(bool loop = true)
{

}

void SoundManager::stopBackgroundMusic()
{

}

float SoundManager::getVolume()
{

}

void SoundManager::setVolume(float volume)
{

}

