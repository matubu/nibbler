#include <raylib.h>

#include "../ILibrary.hpp"

void draw_snake_and_food(const GameData *data, const Snake &snake);

class Library : virtual public ILibrary {
	private:
		Library(const Library &other) { (void)other; *this = other; }
		Library &operator=(const Library &other) { (void)other; return *this; }

	public:
		Library() { }
		~Library() { }

		void createWindow(const GameData *data) {
			SetTraceLogLevel(LOG_NONE);
			InitWindow(data->width * Tile::TILE_SIZE,
						data->height * Tile::TILE_SIZE,
						"Nibbler - RAYLIB");
		}

		void draw(const GameData *data) {
			BeginDrawing();

			ClearBackground(BLACK);

			for (auto &snake : data->snakes) {
				draw_snake_and_food(data, snake);
			}
			EndDrawing();
		}

		i64 getEvents() {
			if (IsKeyDown(KEY_W))
				return Event::W;
			if (IsKeyDown(KEY_A))
				return Event::A;
			if (IsKeyDown(KEY_S))
				return Event::S;
			if (IsKeyDown(KEY_D))
				return Event::D;
			if (IsKeyDown(KEY_RIGHT))
				return Event::RIGHT;
			if (IsKeyDown(KEY_LEFT))
				return Event::LEFT;
			if (IsKeyDown(KEY_UP))
				return Event::UP;
			if (IsKeyDown(KEY_DOWN))
				return Event::DOWN;
			if (IsKeyDown(KEY_R))
				return Event::RESET;
			if (IsKeyDown(KEY_ESCAPE))
				return Event::QUIT;
			if (IsKeyDown(KEY_ONE))
				return Event::LIB1;
			if (IsKeyDown(KEY_TWO))
				return Event::LIB2;
			if (IsKeyDown(KEY_THREE))
				return Event::LIB3;
			return -1;
		}

		void closeWindow() {
			CloseWindow();
		}
};

Library lib;

#ifdef __cplusplus
extern "C" {
#endif

void createWindow(const GameData *data) {
	lib.createWindow(data);
}

void draw(const GameData *data) {
	lib.draw(data);
	return ;
}

// Collect all the events
i64	getEvents() {
	return lib.getEvents();
}

void closeWindow() {
	lib.closeWindow();
}

#ifdef __cplusplus
}
#endif

// Utils

void draw_snake_and_food(const GameData *data, const Snake &snake) {
	Rectangle rec;

	for (u64 i = 0; i < snake.size(); i++) {
		auto &part = snake[i];
		rec = {
			(float)(part.x * Tile::TILE_SIZE),
			(float)(part.y * Tile::TILE_SIZE),
			(float)(Tile::TILE_SIZE),
			(float)(Tile::TILE_SIZE)
		};
		// Head
		if (i == 0)
			DrawRectangleRec(rec, Fade(RED, 0.5f));
		// Tail
		else if (i == snake.size() - 1)
			DrawRectangleRec(rec, Fade(GREEN, .8f));
		// Body
		else
			DrawRectangleRec(rec, Fade(GREEN, .5f));
	}
	rec = {
		(float)(data->food.x * Tile::TILE_SIZE),
		(float)(data->food.y * Tile::TILE_SIZE),
		(float)(Tile::TILE_SIZE),
		(float)(Tile::TILE_SIZE)
	};
	DrawRectangleRec(rec, Fade(RED, 0.5f));
}