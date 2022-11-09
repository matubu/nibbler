#pragma once

#include <iostream>

#include "types.hpp"
#include "Tiles.hpp"
#include "Bot.hpp"
#include "Event.hpp"
#include "Audio.hpp"

void die(const string &s) {
	std::cerr << "\x1b[1;91merror\x1b[0m: " << s << std::endl;
	exit(1);
}

u64 get_micro() {
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

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

void	usage() {
	// TODO
	// --players WASD,BOT,ARROWS
	// --server
	// --connect <ip:port>
	std::cout << "Usage:" << std::endl;
	std::cout << "   ./nibbler <width> <height> [options]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "   <width>              width in cell" << std::endl;
	std::cout << "   <height>             height in cell" << std::endl;
	std::cout << "   --no-music           disable music" << std::endl;
	std::cout << "   --multiplayer        enable multiplayer mode" << std::endl;
	std::cout << "   --bot      		  enable bot mode" << std::endl;
	std::cout << "   --speed <speed>      the speed at which the snakes move" << std::endl;
	exit(0);
}

struct Snake {
	vector<SnakePart>	parts;
	Vec2				direction;
	bool				isDead;
	bool				isBot;
	u64					score;

	// x and y from the center of the snake
	Snake(u64 x, u64 y, bool isBot = false): isDead(false), isBot(isBot), score(0) {
		this->parts.push_back(SnakePart(x, y - 2));
		this->parts.push_back(SnakePart(x, y - 1));
		this->parts.push_back(SnakePart(x, y    ));
		this->parts.push_back(SnakePart(x, y + 1));
		this->direction = Vec2(0, -1);
	}

	void die() {
		this->isDead = true;
	}

	// Returns true if the snake has eaten
	bool updateSnake(const Tiles &tiles) {
		if (this->isBot) {
			this->direction = Bot::getDirection(tiles, this->parts[0]);
		}

		i64 newX = this->parts[0].x + this->direction.x;
		i64 newY = this->parts[0].y + this->direction.y;

		if (tiles.isOutOfBounds(newX, newY) || tiles.at(newX, newY).type == TileType::SNAKE) {
			this->die();
			return false;
		}

		bool eat = tiles.at(newX, newY).type == TileType::FOOD;
		this->parts.insert(this->parts.begin(), SnakePart(newX, newY, eat));

		if (eat) {
			++this->score;
		} else {
			this->parts.pop_back();
		}
		return eat;
	}

	u64 size() const {
		return this->parts.size();
	}
	SnakePart &operator[](u64 i) {
		return this->parts[i];
	}
	const SnakePart &operator[](u64 i) const {
		return this->parts[i];
	}

	vector<SnakePart>::const_iterator begin() const {
		return this->parts.begin();
	}
	vector<SnakePart>::const_iterator end() const {
		return this->parts.end();
	}
};

struct GameData {
	static const u64 DEFAULT_SPEED = 20;

	u64 width;
	u64 height;
	bool multiplayer = false;
	bool bot = false;
	u64 speed;

	bool			gameOver;
	vector<Snake>	snakes;

	Vec2	food;

	AudioManager audioManager;

	u64		resetRestrict = 0; 


	GameData(int ac, char **av) {
		this->parseOptions(ac, av);

		this->reset();
	}

	void	parseOptions(int ac, char **av) {
		if (ac < 3) {
			usage();
		}

		try {
			this->width = std::stoul(av[1]);
			this->height = std::stoul(av[2]);
		}
		catch (const std::exception &e) {
			usage();
		}

		if (this->width < 10 || this->width >= 256
			|| this->height < 10 || this->height >= 256) {
			die("width and height must be >=10 and <256");
		}

		this->speed = DEFAULT_SPEED;

		for (int i = 3 ; i < ac ; i++)
		{
			string	option(av[i]);

			if (option == "--no-music") {
				this->audioManager.setGlobalVolume(0);
			}
			else if (option == "--multiplayer") {
				this->multiplayer = true;
			}
			else if (option == "--bot") {
				this->bot = true;
			}
			else if (option == "--speed") {
				if (av[++i] == NULL) {
					usage();
				}
				try {
					int speed = std::stoi(av[i]);
					if (speed < 1) {
						throw std::exception();
					}
					this->speed = speed;
				}
				catch (const std::exception &e) {
					die("speed must be a valid integer greater than 0");
				}
			}
			else {
				usage();
			}
		}
	}

	void reset() {
	// 	when you keep R pressed, it fires reset() to much
	// 	so I apply 200 ms security
		u64 now = get_micro() / 1000;
		if (now > this->resetRestrict) {
			this->resetRestrict = now + 100;
		}
		else return ;
		this->gameOver = false;

		if (this->multiplayer || this->bot) {
			this->snakes = vector<Snake>({
				Snake(this->width / 2 - 3, this->height / 2),
				Snake(this->width / 2 + 3, this->height / 2, this->bot),
			});
		} else {
			this->snakes = vector<Snake>({
				Snake(this->width / 2, this->height / 2),
			});
		}

		this->spawnFood();

		this->audioManager.play("music", 50.0f, true);
	}

	Tiles getTiles() const {
		Tiles tiles(this->width, this->height);

		tiles.at(this->food) = Tile(TileType::FOOD);
		for (auto &snake : this->snakes) {
			for (u64 i = 0; i < snake.size(); i++) {
				tiles.at(snake[i]) = Tile(TileType::SNAKE);
			}
		}
		return (tiles);
	}

	void spawnFood() {
		Tiles tiles = this->getTiles();

		srand(time(NULL) + clock());
		while (1) {
			Vec2 food(rand() % width, rand() % height);

			if (tiles.at(food).type == TileType::SNAKE) {
				continue ;
			}
			this->food = food;
			break;
		}
	}

	void setGameOver() {
		this->gameOver = true;
		this->audioManager.stop("music");
	}

	void update() {
		if (this->gameOver) {
			return;
		}

		bool allDead = true;

		for (auto &snake : this->snakes) {
			if (snake.isDead) {
				continue;
			}

			Tiles tiles = this->getTiles();
			bool eat = snake.updateSnake(tiles);

			if (eat) {
				this->spawnFood();
				this->audioManager.play("eat");
			}

			if (!snake.isDead) {
				allDead = false;
			} else {
				this->audioManager.play("die");
			}
		}

		if (allDead) {
			this->setGameOver();
		}
	}

	/*
	 * Returns false if the snake does not exist
	 */
	bool changeDirection(u64 snakeId, Vec2 direction) {
		if (snakeId >= this->snakes.size()) {
			return false;
		}

		Snake &snake = this->snakes[snakeId];
		if (snake[0].x + direction.x == snake[1].x
		 && snake[0].y + direction.y == snake[1].y) {
			return true;
		}
		snake.direction = direction;
		return true;
	}

	string getScoreText() const {
		if (this->snakes.size() == 0) {
			return "Score: [no player]";
		}
		string text = "Score: ";
		bool first = true;
		for (auto snake : this->snakes) {
			if (!first) {
				text += " | ";
			}
			text += std::to_string(snake.score);
			first = false;
		}
		return text;
	}
};