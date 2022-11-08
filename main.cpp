#include <cmath>
#include "Lib.hpp"

int main(int ac, char **av) {

	GameData data(ac, av);
	Lib lib(Lib::LIB1, &data);

	u64 nextUpdate = 0;

	while (1) {
		i64 event = lib.getEvents();
		// Handle event
		switch (event) {
			case Event::UP:
				data.changeDirection(0, Vec2(0, -1));
				break;
			case Event::DOWN:
				data.changeDirection(0, Vec2(0, 1));
				break;
			case Event::LEFT:
				data.changeDirection(0, Vec2(-1, 0));
				break;
			case Event::RIGHT:
				data.changeDirection(0, Vec2(1, 0));
				break;
			case Event::W:
				data.changeDirection(1, Vec2(0, -1));
				break;
			case Event::S:
				data.changeDirection(1, Vec2(0, 1));
				break;
			case Event::A:
				data.changeDirection(1, Vec2(-1, 0));
				break;
			case Event::D:
				data.changeDirection(1, Vec2(1, 0));
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

		u64 now = get_micro();
		if (now > nextUpdate) {
			data.update();
			nextUpdate = now + (1000000 / data.speed) - (now - nextUpdate);
			nextUpdate = fmax(nextUpdate, now);
		}
		lib.draw(&data);
	}
}