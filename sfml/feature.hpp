#pragma once

#include "../GameData.hpp"

#include <queue>
#include <map>

struct BfsComparator {
	Vec2 goal;

	BfsComparator(Vec2 goal) : goal(goal) {}

	bool operator()(Vec2 a, Vec2 b)
	{
		return abs(a.x - goal.x) + abs(a.y - goal.y) >
			abs(b.x - goal.x) + abs(b.y - goal.y);
	}
};

typedef
	std::priority_queue<Vec2, std::vector<Vec2>, BfsComparator>
BfsQueue;

void bfs_add(
	BfsQueue &queue, std::map<Vec2, Vec2> &parent,
	Vec2 pos, const Vec2 &parentPos
) {
	if (parent.find(pos) != parent.end()) {
		return ;
	}

	queue.push(pos);
	parent[pos] = parentPos;
}

Vec2 bfs(const GameData *data) {
	BfsComparator cmp(data->snake[0]);
	BfsQueue queue(cmp);
	std::map<Vec2, Vec2> parent;

	queue.push(data->snake[0]);

	while (!queue.empty()) {
		Vec2 pos = queue.top();
		queue.pop();

		if (pos.x < 0 || pos.x >= data->width || pos.y < 0 || pos.y >= data->height) {
			continue;
		}

		if ((data->snake[0] != pos && contains(data->snake, pos))) {
			continue;
		}

		if (contains(data->food, pos)) {
			while (data->snake[0] != parent[pos]) {
				pos = parent[pos];
			}
			return pos;
		}

		bfs_add(queue, parent, Vec2(pos.x + 1, pos.y), pos);
		bfs_add(queue, parent, Vec2(pos.x - 1, pos.y), pos);
		bfs_add(queue, parent, Vec2(pos.x, pos.y + 1), pos);
		bfs_add(queue, parent, Vec2(pos.x, pos.y - 1), pos);
	}

	std::cout << "No path found" << std::endl;
	return data->snake[0];
}

void smartMove(const GameData *data, vector<Event> &events) {
	static Vec2 next = data->snake[0];

	if (data->snake[0] == next) {
		std::cout << "Starting bfs" << std::endl;
		next = bfs(data);
		std::cout << "Curr: " << data->snake[0].x << " " << data->snake[0].y << std::endl;
		std::cout << "Next: " << next.x << " " << next.y << std::endl;
	}

	if (next.x > data->snake[0].x) {
		events.push_back(Event(Event::RIGHT));
	} else if (next.x < data->snake[0].x) {
		events.push_back(Event(Event::LEFT));
	} else if (next.y > data->snake[0].y) {
		events.push_back(Event(Event::DOWN));
	} else if (next.y < data->snake[0].y) {
		events.push_back(Event(Event::UP));
	}
}