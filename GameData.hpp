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

void	usageMessage() {
	std::cout << "+--------------------------------------------+" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "Mandatory: ./nibbler <width >= 10> <height >= 10>" << std::endl;
	std::cout << "Options: ./nibbler <width >= 10> <height >= 10> [no-music, multiplayer, speed=<number>]" << std::endl;
	std::cout << "+--------------------------------------------+" << std::endl;
	exit(EXIT_FAILURE);
}

struct GameData {
	static const u64 TILE_SIZE = 50;
	static const u64 DEFAULT_SPEED = 15;

	u64 width;
	u64 height;
	u64 speed;

	bool gameOver;
	vector<SnakePart> snake;
	Vec2 direction;

	vector<SnakePart> snake2;
	Vec2 direction2;

	Vec2 food;

	bool music = true;
	bool multiplayer = false;

	Audio ambientAudio;
	Audio appleBitAudio;
	Audio gameOverAudio;

	GameData(int ac, char **av):
		ambientAudio(Audio("./sounds/ambient_music.wav")),
		appleBitAudio(Audio("./sounds/apple_bit.wav")),
		gameOverAudio(Audio("./sounds/game_over.wav"))
	{
		initOptions(ac, av);

		if (music) {
			this->ambientAudio.setLoop(true);
			this->ambientAudio.setVolume(50);
		}

		this->reset();
	}

	void	initOptions(int ac, char **av) {
		if (ac < 3)
			usageMessage();

		try {
			this->width = std::stoi(av[1]);
			this->height = std::stoi(av[2]);
		}
		catch (const std::exception &e) {
			usageMessage();
		}

		if (width < 10 || height < 10)
			usageMessage();

		this->speed = DEFAULT_SPEED;

		if (ac < 4)
			return ;

		for (int i = 3 ; i < ac ; i++)
		{
			std::string	option(av[i]);

			if (option == "no-music")
				this->music = false;
			if (option == "multiplayer")
				this->multiplayer = true;
			if (option.substr(0, option.find("=")) == "speed")
			{
				try {
					this->speed = std::stoi(option.substr(option.find("=") + 1));
				}
				catch (const std::exception &e) {
					this->speed = DEFAULT_SPEED;
				}
			}
		}

	}

	void reset() {
		this->gameOver = false;

		this->snake.clear();
		this->snake.push_back(SnakePart(width / 2, height / 2 - 2));
		this->snake.push_back(SnakePart(width / 2, height / 2 - 1));
		this->snake.push_back(SnakePart(width / 2, height / 2    ));
		this->snake.push_back(SnakePart(width / 2, height / 2 + 1));
		this->direction = Vec2(0, -1);

		if (this->multiplayer) {
			this->snake2.clear();
			this->snake2.push_back(SnakePart(width / 2 + 10, height / 2 - 2));
			this->snake2.push_back(SnakePart(width / 2 + 10, height / 2 - 1));
			this->snake2.push_back(SnakePart(width / 2 + 10, height / 2    ));
			this->snake2.push_back(SnakePart(width / 2 + 10, height / 2 + 1));
			this->direction2 = Vec2(0, -1);
		}

		srand(time(NULL) + clock());
		this->spawnFood();

		if (music) {
			this->ambientAudio.stop();
			this->ambientAudio.play();
		}
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
		if (music) {
			this->ambientAudio.stop();
			this->gameOverAudio.play();
		}
	}

	void updateSnake(bool multi) {
		if (this->gameOver) {
			return;
		}

		vector<SnakePart>	*updateSnake = &this->snake;
		Vec2				*updateDirection = &this->direction;

		if (multi)
		{
			updateSnake = &this->snake2;
			updateDirection = &this->direction2;
		}

		i64 newX = (*updateSnake)[0].x + (*updateDirection).x;
		i64 newY = (*updateSnake)[0].y + (*updateDirection).y;

		if (newX < 0 || newX >= this->width || newY < 0 || newY >= this->height
			|| contains((*updateSnake), Vec2(newX, newY))) {
			this->setGameOver();
			return;
		}
		// Checks collision between players
		if (this->multiplayer)
		{
			if (updateSnake == &this->snake && contains(this->snake2, Vec2(newX, newY))) // Check address ?
			{
				this->setGameOver();
				return;
			}
			else if (updateSnake == &this->snake2 && contains(this->snake, Vec2(newX, newY))) // Check address ?
			{
				this->setGameOver();
				return;
			}
		}

		bool eat = this->food == Vec2(newX, newY);
		(*updateSnake).insert((*updateSnake).begin(), SnakePart(newX, newY, eat));

		if (eat) {
			this->spawnFood();
			if (music) {
				this->appleBitAudio.stop();
				this->appleBitAudio.play();
			}
		} else {
			(*updateSnake).pop_back();
		}
	}

	void changeDirection(i64 x, i64 y, bool multiKey) {
		// If mutiplayer mode enabled
		if (this->multiplayer) {
			if (!multiKey && (this->snake[0].x + x != this->snake[1].x
				|| this->snake[0].y + y != this->snake[1].y)) {
				this->direction = Vec2(x, y);
			}
			if (multiKey && (this->snake2[0].x + x != this->snake2[1].x
				|| this->snake2[0].y + y != this->snake2[1].y)) {
				this->direction2 = Vec2(x, y);
			}
		}
		// No multiplayer mode enabled
		else {
			// Block opposite direction
			if (this->snake[0].x + x != this->snake[1].x || this->snake[0].y + y != this->snake[1].y) {
				this->direction = Vec2(x, y);
			}
		}
	}
};