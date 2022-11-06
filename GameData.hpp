#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>

#include "Event.hpp"
#include "Audio.hpp"

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
	template<typename T>
	bool operator == (const T &other) const {
		return x == other.x && y == other.y;
	}
	template<typename T>
	bool operator != (const T &other) const {
		return !(*this == other);
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

// struct Snake {
// 	vector<SnakePart> parts;

// 	Snake() {
// 		this->snake.push_back(SnakePart(width / 2, height / 2 - 2));
// 		this->snake.push_back(SnakePart(width / 2, height / 2 - 1));
// 		this->snake.push_back(SnakePart(width / 2, height / 2    ));
// 		this->snake.push_back(SnakePart(width / 2, height / 2 + 1));
// 	}
// };

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
	static const u64 TILE_SIZE = 50;
	static const u64 DEFAULT_SPEED = 10;

	u64 width;
	u64 height;
	u64 speed;

	bool gameOver;
	vector<SnakePart> snake;
	Vec2 direction;
	Vec2 food;

	Audio ambientAudio;
	Audio appleBitAudio;
	Audio gameOverAudio;

	GameData(u64 width, u64 height):
		ambientAudio(Audio("./sounds/ambient_music.wav")),
		appleBitAudio(Audio("./sounds/apple_bit.wav")),
		gameOverAudio(Audio("./sounds/game_over.wav"))
	{
		if (width < 10 || height < 10) {
			throw "Width and height must be at least 10x10";
		}

		this->width = width;
		this->height = height;
		this->speed = DEFAULT_SPEED;

		// this->ambientAudio = 
		// this->appleBitAudio = 
		// this->gameOverAudio 
		this->ambientAudio.setLoop(true);
		this->ambientAudio.setVolume(50);

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

		srand(time(NULL) + clock());
		this->spawnFood();

		this->ambientAudio.stop();
		this->ambientAudio.play();
	}

	void spawnFood() {
		while (1) {
			Vec2 food(rand() % width, rand() % height);
			// Check if the food is not already on the snake
			if (!contains(this->snake, food)) {
				this->food = food;
				break;
			}
		}
	}

	void setGameOver() {
		this->gameOver = true;
		this->ambientAudio.stop();
		this->gameOverAudio.play();
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

		bool eat = this->food == Vec2(newX, newY);
		this->snake.insert(this->snake.begin(), SnakePart(newX, newY, eat));

		if (eat) {
			this->spawnFood();
			this->appleBitAudio.stop();
			this->appleBitAudio.play();
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