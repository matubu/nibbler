#include <map>
#include <iostream>

struct AudioManager {
	std::map<std::string, void *> audios;
	float globalVolume;

	AudioManager(float globalVolume = 100.0f);
	~AudioManager();

	void	setGlobalVolume(float volume);
	void	play(const std::string &name, float volume = 100.0f, bool loop = false);
	void	stop(const std::string &name) const;

private:
	void	*load_music(const std::string &name);
};