#include <stdint.h>
#include <vector>
#include <string>
#include <dlfcn.h>

using std::vector, std::string;

typedef uint64_t u64;
typedef uint8_t byte;

struct GameData {
	enum TileType
	{
		EMPTY,
		SNAKE,
		SNAKE_HEAD,
		SNAKE_TAIL,
		FOOD
	};

	u64 width;
	u64 height;
	TileType *tiles;

	GameData(u64 width, u64 height) {
		this->width = width;
		this->height = height;
		this->tiles = new TileType[width * height];
	}
};

struct Event {
	enum EventType {
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		LIB1,
		LIB2,
		LIB3,
		QUIT,
	};

	EventType	type;
};

struct Lib {

	typedef void (*createWindow_t)(const GameData *data);
	createWindow_t	createWindow;

	typedef void (*draw_t)(const GameData *data);
	draw_t	draw;

	typedef vector<Event> (*getEvents_t)(const GameData *data);
	getEvents_t	getEvents;

	typedef void (*closeWindow_t)(const GameData *data);
	closeWindow_t	closeWindow;

	void *dl;

	Lib(string path) {
		this->load(path);
	}
	
	void load(string path) {
		// Load the library
		this->dl = dlopen(path.c_str(), RTLD_LAZY);
		this->createWindow = (createWindow_t)dlsym(this->dl, "createWindow");
		this->draw = (draw_t)dlsym(this->dl, "draw");
		this->getEvents = (getEvents_t)dlsym(this->dl, "getEvents");
		this->closeWindow = (closeWindow_t)dlsym(this->dl, "closeWindow");
	}

	void unload(const GameData *data) {
		// Unload the library
		this->closeWindow(data);
		dlclose(this->dl);
	}

};

int main() {
	GameData gameData(10, 10);
	Lib lib("./sfml/libsfml.so");

	lib.createWindow(&gameData);
	lib.draw(&gameData);
	for (auto event : lib.getEvents(&gameData)) {
		// Handle event
	}
}