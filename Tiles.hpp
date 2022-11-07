#pragma once

#include <vector>
#include <vector>
#include <string>

#include "types.hpp"

using std::vector, std::string;

enum TileType {
	EMPTY,
	SNAKE,
	FOOD
};

struct Tile {
	TileType type;

	Tile(TileType type = TileType::EMPTY) : type(type) {}
};

struct Tiles {
	vector<vector<Tile>> tiles;

	Tiles(u64 width, u64 height) {
		tiles.resize(height);
		for (auto &row : tiles) {
			row.resize(width);
		}
	}
	bool isOutOfBounds(i64 x, i64 y) const {
		return x < 0 || (u64)x >= tiles[0].size()
			|| y < 0 || (u64)y >= tiles.size();
	}
	bool isOutOfBounds(const Vec2 &pos) const {
		return isOutOfBounds(pos.x, pos.y);
	}
	const Tile &at(u64 x, u64 y) const {
		return tiles[y][x];
	}
	Tile &at(u64 x, u64 y) {
		return tiles[y][x];
	}
	const Tile &at(Vec2 pos) const {
		return at(pos.x, pos.y);
	}
	Tile &at(Vec2 pos) {
		return at(pos.x, pos.y);
	}
};