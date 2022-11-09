#pragma once 

#include "GameData.hpp"

class ILibrary {
	private:
		ILibrary(const ILibrary &other);
		virtual ILibrary &operator=(const ILibrary &other) {
			(void)other;
			return *this;
		};
	public:
		ILibrary() { }
		virtual ~ILibrary() { };

		virtual void createWindow(const GameData *data) = 0;
		virtual void draw(const GameData *data) = 0;
		virtual i64 getEvents() = 0;
		virtual void closeWindow() = 0;
};
