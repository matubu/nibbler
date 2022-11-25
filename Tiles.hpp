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
	static const u64 TILE_SIZE = 30;
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
	Tiles(const Tiles &other) : tiles(other.tiles) {}

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

std::ostream &operator<<(std::ostream &os, const Tiles &tiles) {
	for (const auto &row : tiles.tiles) {
		for (const auto &tile : row) {
			switch (tile.type) {
			case TileType::EMPTY:
				os << '_';
				break;
			case TileType::SNAKE:
				os << 'X';
				break;
			case TileType::FOOD:
				os << 'O';
				break;
			}
		}
		os << std::endl;
	}
	return os;
}
