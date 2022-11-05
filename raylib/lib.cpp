#include <raylib.h>
#include <iostream>

#include "../GameData.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void createWindow(const GameData *data) {
	InitWindow(data->width * GameData::TILE_SIZE,
				data->height * GameData::TILE_SIZE,
				"Nibbler - RAYLIB");
}

void draw(const GameData *data) {
	BeginDrawing();

	ClearBackground(BLACK);

	DrawText("Nibbler using raylib", 10, 10, 20, DARKGRAY);

	EndDrawing();
}

// Collect all the events
vector<Event> getEvents() {
	vector<Event> events;
	if (IsKeyDown(KEY_RIGHT))
		events.push_back(Event(Event::RIGHT));
	if (IsKeyDown(KEY_LEFT))
		events.push_back(Event(Event::LEFT));
	if (IsKeyDown(KEY_UP))
		events.push_back(Event(Event::UP));
	if (IsKeyDown(KEY_DOWN))
		events.push_back(Event(Event::DOWN));
	if (IsKeyDown(KEY_ESCAPE))
		events.push_back(Event(Event::QUIT));
	if (IsKeyDown(KEY_ONE))
		events.push_back(Event(Event::LIB1));
	if (IsKeyDown(KEY_TWO))
		events.push_back(Event(Event::LIB2));
	if (IsKeyDown(KEY_THREE))
		events.push_back(Event(Event::LIB3));
	return events;
}

void closeWindow() {
	CloseWindow();
}

#ifdef __cplusplus
}
#endif