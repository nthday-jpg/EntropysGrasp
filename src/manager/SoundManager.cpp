#include "SoundManager.h"

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