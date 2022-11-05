#include <raylib.h>
#include <iostream>

#include "../GameData.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void createWindow(const GameData *data) {
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "nibbler");
	SetTargetFPS(60);
}

void draw(const GameData *data) {
	BeginDrawing();

	ClearBackground(BLACK);

	DrawText("test text", 10, 10, 20, DARKGRAY);

	EndDrawing();
}

// Collect all the events
vector<Event> getEvents() {
	vector<Event> events;
	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_RIGHT))
			events.push_back(Event(Event::RIGHT));
		if (IsKeyDown(KEY_LEFT))
			events.push_back(Event(Event::LEFT));
		if (IsKeyDown(KEY_UP))
			events.push_back(Event(Event::UP));
		if (IsKeyDown(KEY_DOWN))
			events.push_back(Event(Event::DOWN));
	}
	return events;
}

void closeWindow() {
	CloseWindow();
}

#ifdef __cplusplus
}
#endif