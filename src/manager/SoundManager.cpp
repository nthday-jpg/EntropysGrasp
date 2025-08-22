#include "SoundManager.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

void SoundManager::onPlaySound(const PlaySoundEvent& event)
{
	playSound(event.name, event.loop);
}

void SoundManager::onStopSound(const StopSoundEvent& event)
{
	stopSound(event.name);
}

void SoundManager::onStopAllSounds(const StopAllSoundsEvent&)
{
	stopAllSounds();
}

void SoundManager::onPlayMusic(const PlayMusicEvent& event)
{
	setBackgroundMusic(event.name, event.loop);
	playBackgroundMusic(event.loop);
}

void SoundManager::onStopMusic(const StopMusicEvent&)
{
	stopBackgroundMusic();
}

SoundManager::~SoundManager()
{
	// Clean up all raw pointers
	for (auto& [_, sound] : sounds)
	{
		delete sound;
	}
	sounds.clear();
}

void SoundManager::bindDispatcher(entt::dispatcher* dispatcher) {
	this->dispatcher = dispatcher;
	
	dispatcher->sink<PlaySoundEvent>().connect<&SoundManager::onPlaySound>(this);
	dispatcher->sink<StopSoundEvent>().connect<&SoundManager::onStopSound>(this);
	dispatcher->sink<StopAllSoundsEvent>().connect<&SoundManager::onStopAllSounds>(this);
	dispatcher->sink<PlayMusicEvent>().connect<&SoundManager::onPlayMusic>(this);
	dispatcher->sink<StopMusicEvent>().connect<&SoundManager::onStopMusic>(this);
	
}

void SoundManager::loadSounds(const std::string& sourcePath)
{
	std::ifstream file(sourcePath);
	if (!file) 
	{
		std::cerr << "Error: Could not open sound file: " << sourcePath << std::endl;
		throw std::runtime_error("Failed to open sound file: " + sourcePath);
	}

	nlohmann::json soundData;
	file >> soundData;
	file.close();

	for (auto& [name, path] : soundData.items())
	{
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile(path)) 
		{
			std::cerr << "Error: Could not load sound file: " << path << std::endl;
			throw std::runtime_error("Failed to load sound file: " + path);
		}
		soundBuffers[name] = buffer;

		// Create sound with new and store raw pointer
		sounds[name] = new sf::Sound(soundBuffers[name]);
	}
}

void SoundManager::loadMusics(const std::string& sourcePath)
{
	std::ifstream file(sourcePath);
	if (!file) 
	{
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

void SoundManager::playSound(const std::string& name, bool loop)
{
	auto bufferIt = soundBuffers.find(name);
	if (bufferIt == soundBuffers.end()) 
	{
		std::cerr << "Sound buffer not found: " << name << std::endl;
		return; 
	}

	auto soundIt = sounds.find(name);
	if (soundIt != sounds.end() && soundIt->second != nullptr) 
	{
		soundIt->second->setBuffer(bufferIt->second);
		soundIt->second->setLooping(loop);
		soundIt->second->setVolume(volume);
		soundIt->second->play();
	}
}

void SoundManager::stopSound(const std::string& name)
{
	auto it = sounds.find(name);
	if (it != sounds.end() && it->second != nullptr)
	{
		it->second->stop();
	}
}

void SoundManager::stopAllSounds()
{
	for (auto& [_, sound] : sounds)
	{
		if (sound != nullptr) {
			sound->stop();
		}
	}
}

void SoundManager::setBackgroundMusic(const std::string& filePath, bool loop)
{
	auto it = musicFiles.find(filePath);
	if (it == musicFiles.end())
	{
		std::cerr << "[SoundManager] Background music not found: " << filePath << std::endl;
		return;
	}

	if (!backgroundMusic.openFromFile(it->second))
	{
		std::cerr << "[SoundManager] Failed to load background music file: " << it->second << std::endl;
		return;
	}

	backgroundMusic.setLooping(loop);
	backgroundMusic.setVolume(volume);
}

void SoundManager::playBackgroundMusic(bool loop)
{
	backgroundMusic.setLooping(loop);
	backgroundMusic.setVolume(volume);
	backgroundMusic.play();
}

void SoundManager::stopBackgroundMusic()
{
	backgroundMusic.stop();
}

float SoundManager::getVolume()
{
	return volume;
}

void SoundManager::setVolume(float newVolume)
{
	volume = newVolume;

	// Update all currently playing sounds
	for (auto& [_, sound] : sounds)
	{
		if (sound != nullptr) {
			sound->setVolume(volume);
		}
	}

	backgroundMusic.setVolume(volume);
}

