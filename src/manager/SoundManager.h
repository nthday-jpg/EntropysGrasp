#pragma once
#include <SFML/Audio.hpp>
#include <entt/signal/dispatcher.hpp>
#include <unordered_map>
#include <string>

// Doc file de init cai nay
inline std::unordered_map<std::string, std::string> soundFiles
{
    {"click", "assets/sounds/click.wav"},
    {"background", "assets/sounds/background.ogg"},
    {"explosion", "assets/sounds/explosion.wav"},
    {"jump", "assets/sounds/jump.wav"},
    {"game_over", "assets/sounds/game_over.wav"}
};

class SoundManager
{
private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::unordered_map<std::string, sf::Sound> sounds;
	sf::Music backgroundMusic;

	entt::dispatcher* dispatcher;

    SoundManager() = default;
    SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
    
public:
    static SoundManager& getInstance()
    {
        static SoundManager instance;
        return instance;
	}

	// bind the dispatcher to the sound manager and setup the sounds
	void bindDispatcher(entt::dispatcher* dispatcher);

};