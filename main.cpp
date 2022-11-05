#include "Lib.hpp"

int main() {
	GameData data(20, 20);
	Lib lib(Lib::LIB1, &data);

	int				lastUpdate = 0;
	pair<i64, i64>	dir(0, -1);

	while (1) {
		for (auto event : lib.getEvents()) {
			// Handle event
			switch (event.type) {
				case Event::UP:
					dir = {0, -1};
					break;
				case Event::DOWN:
					dir = {0, 1};
					break;
				case Event::LEFT:
					dir = {-1, 0};
					break;
				case Event::RIGHT:
					dir = {1, 0};
					break;
				case Event::LIB1:
					lib.switchLib(Lib::LIB1, &data);
					break;
				case Event::LIB2:
					lib.switchLib(Lib::LIB2, &data);
					break;
				case Event::LIB3:
					lib.switchLib(Lib::LIB3, &data);
					break;
				case Event::QUIT:
					lib.unload();
					exit(0);
			}
		}

		if (clock() > lastUpdate) {
			data.moveSnake(dir.first, dir.second);
			lastUpdate = clock() + CLOCKS_PER_SEC / 4;
		}

		lib.draw(&data);
	}
}