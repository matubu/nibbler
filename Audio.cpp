#include "Audio.hpp"
#include <SFML/Audio.hpp>

Audio::Audio(std::string path) {
	this->audio = new sf::Music();
	if (!((sf::Music *)this->audio)->openFromFile(path)) {
		std::cerr << "Error: " << "failed to load sound" << std::endl;
		exit(1);
	}
}

void	Audio::play() const {
	((sf::Music *)this->audio)->play();
}

void	Audio::stop() const {
	((sf::Music *)this->audio)->stop();
}

void	Audio::setLoop(bool loop) const {
	((sf::Music *)this->audio)->setLoop(loop);
}

void	Audio::setVolume(float volume) const {
	((sf::Music *)this->audio)->setVolume(volume);
}