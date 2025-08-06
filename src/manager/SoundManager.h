#pragma once
#include "../gameplay/components/SoundEvent.h"
#include <SFML/Audio.hpp>
#include <entt/signal/dispatcher.hpp>
#include <unordered_map>
#include <string>

// Doc file de init cai nay
//inline std::unordered_map<std::string, std::string> soundFiles
//{
//    {"click", "assets/sounds/click.wav"},
//    {"background", "assets/sounds/background.ogg"},
//    {"explosion", "assets/sounds/explosion.wav"},
//    {"jump", "assets/sounds/jump.wav"},
//    {"game_over", "assets/sounds/game_over.wav"}
//};

class SoundManager
{
private:
	std::unordered_map<std::string, std::string> musicFiles;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
	sf::Music backgroundMusic;
	float volume = 100.0f; 

	entt::dispatcher* dispatcher;

    SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
    
	void onPlaySound(const PlaySoundEvent& event);
	void onStopSound(const StopSoundEvent& event);
	void onStopAllSounds(const StopAllSoundsEvent& event);
	void onPlayMusic(const PlayMusicEvent& event);
	void onStopMusic(const StopMusicEvent& event);
public:
    static SoundManager& getInstance()
    {
        static SoundManager instance;
        return instance;
	}

	~SoundManager() = default;

	void bindDispatcher(entt::dispatcher* dispatcher);

	void loadSounds(const std::string& sourcePath);

	void loadMusics(const std::string& sourcePath);

	void playSound(const std::string& name, bool loop = false);
	void stopSound(const std::string& name);
	void stopAllSounds();

	void setBackgroundMusic(const std::string& filePath, bool loop = true);
	void playBackgroundMusic(bool loop = true);
	void stopBackgroundMusic();


	float getVolume();
	void setVolume(float volume);
};