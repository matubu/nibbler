#pragma once

#include "Tiles.hpp"
#include "GameData.hpp"

#include <queue>
#include <set>
#include <map>

namespace Bot {
	struct BfsNode {
		Vec2 pos;
		i64 score;
		u64 depth;

		BfsNode() {}
		BfsNode(Vec2 pos, u64 depth = 0)
			: pos(pos), depth(depth) {
			score = depth;
		}
	};

	struct BfsComparator {
		BfsComparator() {}

		bool operator()(const BfsNode &a, const BfsNode &b)
		{
			return a.score > b.score;
		}
	};

	typedef std::priority_queue<BfsNode, std::vector<BfsNode>, BfsComparator> BfsQueue;

	void bfs_add(
		const Tiles &tiles,
		BfsQueue &queue,
		std::map<Vec2, BfsNode> &parent,
		Vec2 pos,
		const BfsNode &parentNode
	) {
		if (tiles.isOutOfBounds(pos)) {
			// std::cout << "Out of bounds: " << pos.x << " " << pos.y << std::endl;
			return ;
		}

		// TODO calculate in how many turn the snake will be out of this position
		if (tiles.at(pos).type == TileType::SNAKE) {
			// std::cout << "Snake at " << pos.x << " " << pos.y << std::endl;
			return ;
		}

		auto currParentIt = parent.find(pos);
		if (currParentIt != parent.end() && parentNode.depth >= currParentIt->second.depth) {
			// std::cout << "Already visited" << std::endl;
			return ;
		}

		parent[pos] = parentNode;
		queue.push(BfsNode(pos, parentNode.depth + 1));
	}

	u64 dfs_count(Tiles &tiles, Vec2 start) {
		if (tiles.isOutOfBounds(start) || tiles.at(start).type == TileType::SNAKE) {
			return 0;
		}
		tiles.at(start).type = TileType::SNAKE;
		return 1
			+ dfs_count(tiles, Vec2(start.x + 1, start.y))
			+ dfs_count(tiles, Vec2(start.x - 1, start.y))
			+ dfs_count(tiles, Vec2(start.x, start.y + 1))
			+ dfs_count(tiles, Vec2(start.x, start.y - 1));
	}

	u64 danger_level(Tiles tiles, Vec2 head) {
		Vec2 start(-1, -1);
		u64 expected = 0;

		u64 neighbour = 
				+ (!tiles.isOutOfBounds(Vec2(head.x + 1, head.y)) && tiles.at(Vec2(head.x + 1, head.y)).type != TileType::SNAKE)
				+ (!tiles.isOutOfBounds(Vec2(head.x - 1, head.y)) && tiles.at(Vec2(head.x - 1, head.y)).type != TileType::SNAKE)
				+ (!tiles.isOutOfBounds(Vec2(head.x, head.y + 1)) && tiles.at(Vec2(head.x, head.y + 1)).type != TileType::SNAKE)
				+ (!tiles.isOutOfBounds(Vec2(head.x, head.y - 1)) && tiles.at(Vec2(head.x, head.y - 1)).type != TileType::SNAKE);

		if (neighbour <= 1) {
			return -1;
		}

		for (u64 y = 0; y < tiles.tiles.size(); ++y) {
			for (u64 x = 0; x < tiles.tiles[y].size(); ++x) {
				if (tiles.at(x, y).type != TileType::SNAKE) {
					++expected;
					start = Vec2(x, y);
				}
			}
		}

		u64 actual = dfs_count(tiles, start);
		return (expected - actual);
	}

	vector<Vec2> bfs(const Tiles &tiles, const Vec2 &start) {
		BfsQueue queue;
		std::map<Vec2, BfsNode> parent;

		queue.push(BfsNode(start));

		while (!queue.empty()) {
			BfsNode pos = queue.top();
			queue.pop();

			if (tiles.at(pos.pos).type == TileType::FOOD) {
				Vec2 it = pos.pos;
				vector<Vec2> path;

				while (1) {
					path.push_back(it);
					if (it == start) {
						break;
					}
					it = parent[it].pos;
				};

				return path;
			}

			bfs_add(tiles, queue, parent, Vec2(pos.pos.x + 1, pos.pos.y), pos);
			bfs_add(tiles, queue, parent, Vec2(pos.pos.x - 1, pos.pos.y), pos);
			bfs_add(tiles, queue, parent, Vec2(pos.pos.x, pos.pos.y + 1), pos);
			bfs_add(tiles, queue, parent, Vec2(pos.pos.x, pos.pos.y - 1), pos);
		}

		return vector<Vec2>();
	}

	Vec2 getDirection(const Tiles &tiles, const Vec2 &start) {
		Vec2 next = start;
		u64 best_dist = -1;
		u64 best_danger = -1;

		vector<Vec2> dirs{
			Vec2(1, 0),
			Vec2(-1, 0),
			Vec2(0, 1),
			Vec2(0, -1),
		};

		for (auto dir : dirs) {
			Vec2 pos(start.x + dir.x, start.y + dir.y);
			if (tiles.isOutOfBounds(pos) || tiles.at(pos).type == TileType::SNAKE) {
				continue ;
			}

			Tiles copy(tiles);
			copy.at(pos) = TileType::SNAKE;

			u64 danger = danger_level(copy, pos);

			bool is_food = tiles.at(pos).type == TileType::FOOD;

			if (is_food && !danger) {
				next = pos;
				break ;
			}

			u64 dist = is_food ? 1 : bfs(copy, pos).size();

			if (dist == 0) {
				dist = -1;
			}

			if (danger < best_danger || (danger == best_danger && dist <= best_dist)) {
				next = pos;
				best_dist = dist;
				best_danger = danger;
			}
		}

		if (best_danger) {
			std::cout << rand() << " ***TOOK A DANGEROUS PATH***" << std::endl;
		}

		return Vec2(next.x - start.x, next.y - start.y);
	}
}