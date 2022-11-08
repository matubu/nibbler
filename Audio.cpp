#include "Audio.hpp"
#include <SFML/Audio.hpp>

/** AudioManager **/
AudioManager::AudioManager(float globalVolume) {
	this->globalVolume = globalVolume;
}

AudioManager::~AudioManager() {
	for (auto &audio: this->audios) {
		delete ((sf::Music *)audio.second);
	}
}

void	AudioManager::setGlobalVolume(float volume) {
	this->globalVolume = volume;
}

void	*AudioManager::load_music(const std::string &name) {
	std::string path = "sounds/" + name + ".wav";

	sf::Music *audio = new sf::Music();
	if (!audio->openFromFile(path)) {
		std::cerr << "Error: " << "failed to load sound" << std::endl;
		exit(1);
	}

	return audio;
}

void	AudioManager::play(const std::string &name, float volume, bool loop) {
	volume *= globalVolume;
	if (volume == 0.0f) {
		return ;
	}
	auto it = this->audios.find(name);
	if (it == this->audios.end()) {
		std::cout << "load music" << std::endl;
		it = this->audios.insert(std::make_pair(name, this->load_music(name))).first;
	}
	((sf::Music *)it->second)->stop();
	((sf::Music *)it->second)->setLoop(loop);
	((sf::Music *)it->second)->setVolume(volume);
	((sf::Music *)it->second)->play();
}

void	AudioManager::stop(const std::string &name) const {
	auto it = this->audios.find(name);
	if (it == this->audios.end()) {
		return ;
	}
	((sf::Music *)it->second)->stop();
}