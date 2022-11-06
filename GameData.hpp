#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include "Event.hpp"

using std::vector, std::pair, std::make_pair, std::string;

typedef uint64_t u64;
typedef int64_t i64;
typedef uint8_t byte;

bool contains(const vector<pair<i64, i64>> &vec, pair<i64, i64> pair) {
	for (auto p : vec) {
		if (p.first == pair.first && p.second == pair.second) {
			return true;
		}
	}
	return false;
}

struct GameData {
	static const u64 TILE_SIZE = 20;
	static const u64 MOVE_PER_SEC = 15;

	u64 width;
	u64 height;

	bool gameOver;
	vector<pair<i64, i64>> snake;
	vector<pair<i64, i64>> food;
	pair<i64, i64> direction;

	GameData(u64 width, u64 height) {
		if (width < 4 || height < 4) {
			throw "Width and height must be at least 4";
		}

		this->width = width;
		this->height = height;

		this->gameOver = false;

		this->snake.push_back(make_pair(width / 2, height / 2 - 2));
		this->snake.push_back(make_pair(width / 2, height / 2 - 1));
		this->snake.push_back(make_pair(width / 2, height / 2    ));
		this->snake.push_back(make_pair(width / 2, height / 2 + 1));

		srand(time(NULL));
		this->spawnFood();

		this->direction = make_pair(0, -1);
	}

	void spawnFood() {
		// TODO handle no space left on screen
		while (1) {
			pair<i64, i64> food = make_pair(rand() % width, rand() % height);
			// Check if the food is not already on the snake
			if (!contains(this->snake, food)) {
				this->food.push_back(food);
				break;
			}
		}
	}

	void updateSnake() {
		if (this->gameOver) {
			return;
		}

		i64 newX = this->snake[0].first + this->direction.first;
		i64 newY = this->snake[0].second + this->direction.second;

		if (newX < 0 || newX >= this->width || newY < 0 || newY >= this->height
			|| contains(this->snake, make_pair(newX, newY))) {
			this->gameOver = true;
			return;
		}

		if (contains(this->food, make_pair(newX, newY))) {
			this->food.erase(this->food.begin());
			this->spawnFood();
		} else {
			this->snake.pop_back();
		}

		this->snake.insert(this->snake.begin(), make_pair(newX, newY));
	}

	void changeDirection(i64 x, i64 y) {
		// Block opposite direction
		if (this->snake[0].first + x != this->snake[1].first
			|| this->snake[0].second + y != this->snake[1].second) {
			this->direction = make_pair(x, y);
		}
	}
};