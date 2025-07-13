#pragma once
#include <string>
#include <map>
#include <SFML/Window/Keyboard.hpp>  

class GameConfig 
{
public:
	static GameConfig& getInstance()
	{
		static GameConfig instance; 
		return instance;
	}

	// Window settings (const - set at initialization)
	const int windowWidth = 800;
	const int windowHeight = 600;
	const bool fullscreen = false;
	const std::string windowTitle = "Game Window";

	// Performance settings (const - set at initialization)
	const int maxFPS = 60;
	const bool vsyncEnabled = true;  // Controls vertical synchronization

	// Graphics settings (const - set at initialization)
	const bool enableParticleEffects = true;
	const bool enablePostProcessing = true;

	// Audio getters
	/*float getMasterVolume() const { return masterVolume; }
	float getMusicVolume() const { return musicVolume; }
	float getSfxVolume() const { return sfxVolume; }
	bool isMuteAudio() const { return muteAudio; }*/

	// Audio setters with validation
	/*void setMasterVolume(float value) { masterVolume = clamp(value, 0.0f, 1.0f); }
	void setMusicVolume(float value) { musicVolume = clamp(value, 0.0f, 1.0f); }
	void setSfxVolume(float value) { sfxVolume = clamp(value, 0.0f, 1.0f); }
	void setMuteAudio(bool value) { muteAudio = value; }*/
	
	// Key bindings access
	const std::map<sf::Keyboard::Scancode, std::string>* getKeyBindings() const { return &keyBindings; }
	//void setKeyBinding(sf::Keyboard::Scancode key, const std::string& action) { keyBindings[key] = action; }

private:
	GameConfig() = default; // Private constructor for singleton
	GameConfig(const GameConfig&) = delete;
	GameConfig& operator=(const GameConfig&) = delete;

	// Helper function to clamp values
	float clamp(float value, float min, float max) const {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	// Audio settings
	float masterVolume = 1.0f;  // Range from 0.0 to 1.0
	float musicVolume = 0.8f;   // Range from 0.0 to 1.0
	float sfxVolume = 0.8f;     // Range from 0.0 to 1.0
	bool muteAudio = false;     // Mute all audio

	// Key bindings - maps keyboard keys to action strings
	std::map<sf::Keyboard::Scancode, std::string> keyBindings = {
		{sf::Keyboard::Scancode::W, "MOVEUP"},
		{sf::Keyboard::Scancode::A, "MOVELEFT"},
		{sf::Keyboard::Scancode::S, "MOVEDOWN"},
		{sf::Keyboard::Scancode::D, "MOVERIGHT"},
		{sf::Keyboard::Scancode::Space, "DASH"},
		{sf::Keyboard::Scancode::LShift, "CAST"},
		{sf::Keyboard::Scancode::Escape, "PAUSE"}
	};		
};