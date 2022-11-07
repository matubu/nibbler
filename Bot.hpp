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
			return ;
		}

		// TODO calculate in how many turn the snake will be out of this position
		if (tiles.at(pos).type == TileType::SNAKE) {
			return ;
		}

		auto currParentIt = parent.find(pos);
		if (currParentIt != parent.end() && parentNode.depth >= currParentIt->second.depth) {
			return ;
		}

		parent[pos] = parentNode;
		queue.push(BfsNode(pos, parentNode.depth + 1));
	}

	Vec2 bfs(const Tiles &tiles, const Vec2 &start) {
		BfsQueue queue;
		std::map<Vec2, BfsNode> parent;

		queue.push(BfsNode(start));

		while (!queue.empty()) {
			BfsNode pos = queue.top();
			queue.pop();

			if (tiles.at(pos.pos).type == TileType::FOOD) {
				Vec2 it = pos.pos;
				Vec2 tmp;
				while ((tmp = parent[it].pos) != start) {
					it = tmp;
				}
				return it;
			}

			bfs_add(tiles, queue, parent, Vec2(pos.pos.x + 1, pos.pos.y), pos);
			bfs_add(tiles, queue, parent, Vec2(pos.pos.x - 1, pos.pos.y), pos);
			bfs_add(tiles, queue, parent, Vec2(pos.pos.x, pos.pos.y + 1), pos);
			bfs_add(tiles, queue, parent, Vec2(pos.pos.x, pos.pos.y - 1), pos);
		}

		return start;
	}

	Vec2 getDirection(const Tiles &tiles, const Vec2 &start) {
		static Vec2 next;

		next = bfs(tiles, start);
		return Vec2(next.x - start.x, next.y - start.y);
	}
}