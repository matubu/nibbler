#include <SDL2/SDL.h>

#include "../GameData.hpp"

#ifdef __cplusplus
extern "C" {
#endif

SDL_Window		*window = NULL;
SDL_Renderer	*renderer = NULL;

void createWindow(const GameData *data) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		printf("Error initializing SDL: %s\n", SDL_GetError());

	// Create window
	window = SDL_CreateWindow("Nibbler - SDL2",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		data->width * GameData::TILE_SIZE,
		data->height * GameData::TILE_SIZE, 0);

	// Create renderer	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void draw(const GameData *data) {
	if (!window) {
		return ;
	}
	SDL_RenderClear(renderer);
}

// Collect all the events
vector<Event> getEvents(const GameData *data) {
	if (!window) {
		return vector<Event>();
	}
	vector<Event> events;

	// Events management
	SDL_Event event; 
	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
			case SDL_QUIT:
				events.push_back(Event(Event::QUIT));
				break;

			case SDL_KEYDOWN:
				std::cout << event.key.keysym.scancode << std::endl;
				// keyboard API for key pressed
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_1:
						events.push_back(Event(Event::LIB1));
						break;
					case SDL_SCANCODE_2:
						events.push_back(Event(Event::LIB2));
						break;
					case SDL_SCANCODE_3:
						events.push_back(Event(Event::LIB3));
						break;
					case SDL_SCANCODE_UP:
						events.push_back(Event(Event::UP));
						break;
					case SDL_SCANCODE_LEFT:
						events.push_back(Event(Event::LEFT));
						break;
					case SDL_SCANCODE_DOWN:
						events.push_back(Event(Event::DOWN));
						break;
					case SDL_SCANCODE_RIGHT:
						events.push_back(Event(Event::RIGHT));
						break;
					case SDL_SCANCODE_ESCAPE:
						events.push_back(Event(Event::QUIT));
					default:
						break;
				}
			}
	}
	return events;
}

void closeWindow() {
	if (!window)
		return ;
	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();
}

#ifdef __cplusplus
}
#endif