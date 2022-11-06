#include "Lib.hpp"

int main() {
	GameData data(20, 20);
	Lib lib(Lib::LIB1, &data);

	int				nextUpdate = 0;

	while (1) {
		for (auto event : lib.getEvents(&data)) {
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
				case Event::SPEED_UP:
					data.speed++;
					break;
				case Event::SPEED_DOWN:
					if (data.speed > 1)
						data.speed--;
					break;
				case Event::RESET:
					data.reset();
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

		if (clock() > nextUpdate) {
			data.updateSnake();
			nextUpdate = clock() + CLOCKS_PER_SEC / data.speed;
		}

		lib.draw(&data);
	}
}