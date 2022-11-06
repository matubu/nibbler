#include "Lib.hpp"

int main() {
	GameData data(30, 30);
	Lib lib(Lib::LIB1, &data);

	int				lastUpdate = 0;

	while (1) {
		for (auto event : lib.getEvents()) {
			// Handle event
			switch (event.type) {
				case Event::UP:
					data.changeDirection(0, -1);
					break;
				case Event::DOWN:
					data.changeDirection(0, 1);
					break;
				case Event::LEFT:
					data.changeDirection(-1, 0);
					break;
				case Event::RIGHT:
					data.changeDirection(1, 0);
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
			data.updateSnake();
			lastUpdate = clock() + CLOCKS_PER_SEC / GameData::MOVE_PER_SEC;
		}

		lib.draw(&data);
	}
}