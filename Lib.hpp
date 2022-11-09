#pragma once

#include <dlfcn.h>
#include <exception>

#include "ILibrary.hpp"

using std::cerr, std::endl;

struct Lib {
	static const string LIB1, LIB2, LIB3;

	typedef void (*createWindow_t)(const GameData *);
	createWindow_t	createWindow;

	typedef void (*draw_t)(const GameData *);
	draw_t	draw;

	typedef i64 (*getEvents_t)();
	getEvents_t	getEvents;

	typedef void (*closeWindow_t)();
	closeWindow_t	closeWindow;

	void *dl;

	Lib(string path, GameData *data) {
		this->load(path, data);
	}

	void switchLib(string path, GameData *data) {
		this->unload();
		this->load(path, data);
	}

	void *loadSym(string name) {
		void *sym = dlsym(this->dl, name.c_str());
		if (!sym) {
			throw std::runtime_error(dlerror());
		}
		return sym;
	}

	void load(string path, GameData *data) {
		// Load the library
		cerr << "Loading " << path << endl;
		this->dl = dlopen(path.c_str(), RTLD_LAZY);
		if (!this->dl) {
			throw std::runtime_error(dlerror());
		}
		this->createWindow = (createWindow_t)this->loadSym("createWindow");
		this->draw = (draw_t)this->loadSym("draw");
		this->getEvents = (getEvents_t)this->loadSym("getEvents");
		this->closeWindow = (closeWindow_t)this->loadSym("closeWindow");
		this->createWindow(data);
	}

	void unload() {
		// Unload the library
		this->closeWindow();
		if (dlclose(this->dl) < 0) {
			throw std::runtime_error(dlerror());
		}
		this->dl = NULL;
		this->createWindow = NULL;
		this->draw = NULL;
		this->getEvents = NULL;
		this->closeWindow = NULL;
	}
};

const string Lib::LIB1 = "./sfml/libsfml.so";
const string Lib::LIB2 = "./raylib/libraylib.so";
const string Lib::LIB3 = "./sdl2/libsdl2.so";