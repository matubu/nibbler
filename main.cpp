#include "Lib.hpp"

int main() {
	GameData data(10, 10);
	Lib lib(Lib::LIB1, &data);

	while (1) {
		for (auto event : lib.getEvents()) {
			// Handle event
			switch (event.type) {
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

		lib.draw(&data);
	}
}