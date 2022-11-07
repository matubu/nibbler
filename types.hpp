#pragma once

typedef uint64_t u64;
typedef int64_t i64;
typedef uint8_t byte;

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