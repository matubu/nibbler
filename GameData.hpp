#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>

#include <SFML/Audio.hpp>

#include "Event.hpp"

using std::vector, std::pair, std::make_pair, std::string;

typedef uint64_t u64;
typedef int64_t i64;
typedef uint8_t byte;

void die(const string &s) {
	std::cerr << "Error: " << s << std::endl;
	exit(1);
}

struct Vec2 {
	i64 x, y;

	Vec2() : x(0), y(0) {}
	Vec2(i64 x, i64 y) : x(x), y(y) {}

	bool operator < (const Vec2 &other) const {
		return x < other.x || (x == other.x && y < other.y);
	}
};

struct SnakePart: public Vec2 {
	bool isEating;

	SnakePart(i64 x, i64 y, bool isEating = 0)
		: Vec2(x, y), isEating(isEating) {}

	template<typename T>
	bool operator == (const T &other) const {
		return x == other.x && y == other.y;
	}
	template<typename T>
	bool operator != (const T &other) const {
		return !(*this == other);
	}
};

template <typename T>
bool contains(const vector<T> &vec, Vec2 pair) {
	for (auto p : vec) {
		if (p.x == pair.x && p.y == pair.y) {
			return true;
		}
	}
	return false;
}

struct GameData {
	static const u64 TILE_SIZE = 10;
	static const u64 MOVE_PER_SEC = 1000;

	u64 width;
	u64 height;

	bool gameOver;
	vector<SnakePart> snake;
	Vec2 direction;
	vector<Vec2> food;

	sf::Music ambient_music_audio;
	sf::Music apple_bit_audio;
	sf::Music game_over_audio;

	GameData(u64 width, u64 height) {
		if (width < 10 || height < 10) {
			throw "Width and height must be at least 10x10";
		}

		this->width = width;
		this->height = height;

		this->loadSound(this->ambient_music_audio, "./sounds/ambient_music.wav");
		this->loadSound(this->apple_bit_audio, "./sounds/apple_bit.wav");
		this->loadSound(this->game_over_audio, "./sounds/game_over.wav");
		this->ambient_music_audio.setLoop(true);
		this->ambient_music_audio.setVolume(50);

		this->reset();
	}

	void reset() {
		this->gameOver = false;

		this->snake.clear();
		this->snake.push_back(SnakePart(width / 2, height / 2 - 2));
		this->snake.push_back(SnakePart(width / 2, height / 2 - 1));
		this->snake.push_back(SnakePart(width / 2, height / 2    ));
		this->snake.push_back(SnakePart(width / 2, height / 2 + 1));

		this->direction = Vec2(0, -1);

		this->food.clear();
		srand(time(NULL));
		this->spawnFood();

		this->ambient_music_audio.stop();
		this->ambient_music_audio.play();
	}

	void loadSound(sf::Music &sound, string path) {
		if (!sound.openFromFile(path))
			die("failed to load sound");
	}

	void spawnFood() {
		while (1) {
			Vec2 food(rand() % width, rand() % height);
			// Check if the food is not already on the snake
			if (!contains(this->snake, food)) {
				this->food.push_back(food);
				break;
			}
		}
	}

	void setGameOver() {
		this->gameOver = true;
		this->ambient_music_audio.stop();
		this->game_over_audio.play();
	}

	void updateSnake() {
		if (this->gameOver) {
			return;
		}

		i64 newX = this->snake[0].x + this->direction.x;
		i64 newY = this->snake[0].y + this->direction.y;

		if (newX < 0 || newX >= this->width || newY < 0 || newY >= this->height
			|| contains(this->snake, Vec2(newX, newY))) {
			this->setGameOver();
			return;
		}

		bool eat = contains(this->food, Vec2(newX, newY));
		this->snake.insert(this->snake.begin(), SnakePart(newX, newY, eat));

		if (eat) {
			this->food.erase(this->food.begin());
			this->spawnFood();
			this->apple_bit_audio.stop();
			this->apple_bit_audio.play();
		} else {
			this->snake.pop_back();
		}
	}

	void changeDirection(i64 x, i64 y) {
		// Block opposite direction
		if (this->snake[0].x + x != this->snake[1].x
			|| this->snake[0].y + y != this->snake[1].y) {
			this->direction = Vec2(x, y);
		}
	}
};