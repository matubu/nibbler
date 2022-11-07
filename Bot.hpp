#pragma once

#include "../GameData.hpp"

#include <queue>
#include <set>
#include <map>

struct BfsNode {
	Vec2 pos;
	u64 dist;
	u64 score;
	u64 depth;

	BfsNode() {}
	BfsNode(Vec2 pos, Vec2 goal, u64 depth = 0)
		: pos(pos), depth(depth) {
		dist = abs(pos.x - goal.x) + abs(pos.y - goal.y);
		score = dist + rand() % 2;
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
	const GameData *data,
	Vec2 goal,
	BfsQueue &queue,
	std::map<Vec2, BfsNode> &parent,
	Vec2 pos,
	const BfsNode &parentNode
) {
	auto snakePartIt = std::find(data->snake.begin(), data->snake.end(), pos);
	if (snakePartIt != data->snake.end()
		&& parentNode.depth < data->snake.end() - snakePartIt) {
		return ;
	}

	if (pos.x < 0 || pos.x >= data->width
		|| pos.y < 0 || pos.y >= data->height) {
		return ;
	}

	auto currParentIt = parent.find(pos);
	if (currParentIt != parent.end() && parentNode.depth >= currParentIt->second.depth) {
		return ;
	}

	parent[pos] = parentNode;
	queue.push(BfsNode(pos, goal, parentNode.depth + 1));
}

Vec2 bfs(const GameData *data, const Vec2 &goal) {
	BfsQueue queue;
	std::map<Vec2, BfsNode> parent;

	queue.push(BfsNode(data->snake[0], goal));

	while (!queue.empty()) {
		BfsNode pos = queue.top();
		queue.pop();

		if (pos.dist == 0) {
			Vec2 it = pos.pos;
			Vec2 tmp;
			while (data->snake[0] != (tmp = parent[it].pos)) {
				it = tmp;
			}
			return it;
		}

		bfs_add(data, goal, queue, parent, Vec2(pos.pos.x + 1, pos.pos.y), pos);
		bfs_add(data, goal, queue, parent, Vec2(pos.pos.x - 1, pos.pos.y), pos);
		bfs_add(data, goal, queue, parent, Vec2(pos.pos.x, pos.pos.y + 1), pos);
		bfs_add(data, goal, queue, parent, Vec2(pos.pos.x, pos.pos.y - 1), pos);
	}

	return data->snake[0];
}

void smartMove(const GameData *data, vector<Event> &events) {
	static Vec2 next;
	static Vec2 lastPos = data->snake[0];

	if (data->snake[0] != lastPos) {
		// if (data->snake.size() > 50) {
		// 	next = bfs(data, data->snake[0]);
		// } else {
			next = bfs(data, data->food);
		// }
		lastPos = data->snake[0];

		if (next.x > data->snake[0].x) {
			events.push_back(Event::RIGHT);
		} else if (next.x < data->snake[0].x) {
			events.push_back(Event::LEFT);
		} else if (next.y > data->snake[0].y) {
			events.push_back(Event::DOWN);
		} else if (next.y < data->snake[0].y) {
			events.push_back(Event::UP);
		}
	}
}