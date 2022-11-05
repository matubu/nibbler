#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include "Event.hpp"

using std::vector, std::string;

typedef uint64_t u64;
typedef uint8_t byte;

struct GameData {
	enum TileType
	{
		EMPTY,
		SNAKE_HEAD,
		SNAKE_BODY,
		SNAKE_TAIL,
		FOOD
	};

	static const u64 TILE_SIZE = 30;

	u64 width;
	u64 height;
	TileType *tiles;

	GameData(u64 width, u64 height) {
		if (width < 4 || height < 4) {
			throw "Width and height must be at least 4";
		}

		this->width = width;
		this->height = height;
		this->tiles = new TileType[width * height];
		if (!this->tiles) {
			throw "Failed to allocate memory for tiles";
		}

		this->setTile(width / 2, height / 2 - 2, SNAKE_HEAD);
		this->setTile(width / 2, height / 2 - 1, SNAKE_BODY);
		this->setTile(width / 2, height / 2 + 0, SNAKE_BODY);
		this->setTile(width / 2, height / 2 + 1, SNAKE_TAIL);
	}

	void setTile(u64 x, u64 y, TileType type) {
		this->tiles[x + y * this->width] = type;
	}

	TileType getTile(u64 x, u64 y) const {
		return this->tiles[x + y * this->width];
	}
};