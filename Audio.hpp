#include <iostream>

struct Audio {
	void	*audio;

	Audio(const std::string &path);
	void	play() const;
	void	stop() const;
	void	setLoop(bool loop) const;
	void	setVolume(float volume) const;
};