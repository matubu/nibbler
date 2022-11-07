#include "Audio.hpp"
#include <SFML/Audio.hpp>

/** Audio */
Audio::Audio(const std::string &name) {
	std::string path = "sounds/" + name + ".wav";

	this->audio = new sf::Music();

	if (!((sf::Music *)this->audio)->openFromFile(path)) {
		std::cerr << "Error: " << "failed to load sound" << std::endl;
		exit(1);
	}
}

void	Audio::play() const {
	this->stop();
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

/** AudioManager **/
AudioManager::AudioManager(float globalVolume) {
	this->globalVolume = globalVolume;
}

void	AudioManager::setGlobalVolume(float volume) {
	this->globalVolume = volume;
}

void	AudioManager::play(const std::string &name, float volume, bool loop) {
	volume *= globalVolume;
	if (volume == 0.0f) {
		return ;
	}
	auto it = this->audios.find(name);
	if (it == this->audios.end()) {
		it = this->audios.insert(std::make_pair(name, Audio(name))).first;
	}
	it->second.setLoop(loop);
	it->second.setVolume(volume);
	it->second.play();
}

void	AudioManager::stop(const std::string &name) const {
	auto it = this->audios.find(name);
	if (it == this->audios.end()) {
		return ;
	}
	it->second.stop();
}