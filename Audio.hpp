#include <map>
#include <iostream>

struct Audio {
	void	*audio;

	Audio(const std::string &path);

	void	play() const;
	void	stop() const;
	void	setLoop(bool loop) const;
	void	setVolume(float volume) const;
};

struct AudioManager {
	std::map<std::string, Audio> audios;
	float globalVolume;

	AudioManager(float globalVolume = 100.0f);

	void	setGlobalVolume(float volume);
	void	play(const std::string &name, float volume = 100.0f, bool loop = false);
	void	stop(const std::string &name) const;
};