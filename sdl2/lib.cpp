#include <SDL2/SDL.h>
#include <map>

#include "../ILibrary.hpp"

void	drawRect(i64 x, i64 y);
void draw_snake(const Snake &snake);

SDL_Color		grey = {132, 215, 117, 0};
SDL_Color		green = {132, 237, 125, 0};
SDL_Color		red = {242, 130, 106, 0};

class Library : virtual public ILibrary {
	private:
		Library(const Library &other) { (void)other; *this = other; }
		Library &operator=(const Library &other) { (void)other; return *this; }

	public:
		Library() { }
		~Library() { }

		static const u64 HELP_FONT_SIZE = 15 * 2;
		static const u64 TEXT_FONT_SIZE = 24 * 2;
		static const u64 TITLE_FONT_SIZE = 48 * 2;

		SDL_Window					*window = NULL;
		SDL_Renderer				*renderer = NULL;

		void createWindow(const GameData *data) {
			if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
				printf("Error initializing SDL: %s\n", SDL_GetError());

			// Create window
			this->window = SDL_CreateWindow("Nibbler - SDL2",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				data->width * Tile::TILE_SIZE,
				data->height * Tile::TILE_SIZE, 0);

			// Create renderer	
			this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
		}

		void draw(const GameData *data) {
			if (!this->window || !this->renderer)
				return ;
			SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
			SDL_RenderClear(this->renderer);

			for (auto &snake : data->snakes) {
				draw_snake(snake);
			}
			SDL_SetRenderDrawColor(this->renderer, red.r, red.g, red.b, red.a);
			drawRect(data->food.x, data->food.y);
			SDL_RenderPresent(this->renderer);
		}

		i64 getEvents() {
			if (!this->window)
				return Event::QUIT;
			// Events management
			SDL_Event	event; 
			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
					case SDL_QUIT:
						return Event::QUIT;
					case SDL_KEYDOWN:
						// keyboard API for key pressed
						switch (event.key.keysym.scancode) {
							case SDL_SCANCODE_1:
								return Event::LIB1;
							case SDL_SCANCODE_2:
								return Event::LIB2;
							case SDL_SCANCODE_3:
								return Event::LIB3;
							case SDL_SCANCODE_UP:
								return Event::UP;
							case SDL_SCANCODE_LEFT:
								return Event::LEFT;
							case SDL_SCANCODE_DOWN:
								return Event::DOWN;
							case SDL_SCANCODE_RIGHT:
								return Event::RIGHT;
							case SDL_SCANCODE_W:
								return Event::W;
							case SDL_SCANCODE_A:
								return Event::A;
							case SDL_SCANCODE_S:
								return Event::S;
							case SDL_SCANCODE_D:
								return Event::D;
							case SDL_SCANCODE_ESCAPE:
								return Event::QUIT;
							case 46:
								return Event::SPEED_UP;
							case 45:
								return Event::SPEED_DOWN;
							case SDL_SCANCODE_R:
								return Event::RESET;
							default:
								return -1;
						}
						default:
							break;
				}
			}
			return -1;
		}

		void closeWindow() {
			if (this->renderer)
				SDL_DestroyRenderer(renderer);
			if (this->window)
				SDL_DestroyWindow(window);
			SDL_Quit();
		}
};

Library lib;

#ifdef __cplusplus
extern "C" {
#endif

void createWindow(const GameData *data) {
	lib.createWindow(data);
	return ;
}

void draw(const GameData *data) {
	lib.draw(data);
	return ;
}

i64	getEvents() {
	return lib.getEvents();
}

void closeWindow() {
	lib.closeWindow();
	return ;
}

#ifdef __cplusplus
}
#endif

void draw_snake(const Snake &snake) {
	for (u64 i = 0; i < snake.size(); i++) {
		auto &part = snake[i];
		
		// Head
		if (i == 0) 
			SDL_SetRenderDrawColor(lib.renderer, red.r, red.g, red.b, red.a);
		// Tail
		else if (i == snake.size() - 1)
			SDL_SetRenderDrawColor(lib.renderer, grey.r, grey.g, grey.b, grey.a);
		// Body
		else
			SDL_SetRenderDrawColor(lib.renderer, green.r, green.g, green.b, green.a);
		drawRect(part.x, part.y);
	}
}

void	drawRect(i64 x, i64 y) {
	SDL_Rect r;
	r.x = x * Tile::TILE_SIZE; r.y = y * Tile::TILE_SIZE;
	r.w = Tile::TILE_SIZE; r.h = Tile::TILE_SIZE;

	// Render rect
	SDL_RenderFillRect(lib.renderer, &r);
}