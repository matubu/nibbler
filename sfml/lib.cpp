#include <SFML/Graphics.hpp>
#include <iostream>

#include "../GameData.hpp"

sf::RenderWindow *window = NULL;

#ifdef __cplusplus
extern "C" {
#endif

void createWindow(const GameData *data) {
	window = new sf::RenderWindow(
		sf::VideoMode(
			data->width * GameData::TILE_SIZE,
			data->height * GameData::TILE_SIZE
		),
		"Nibbler - SFML"
	);
}

void draw(const GameData *data) {
	if (!window) {
		return ;
	}

	window->clear(sf::Color::Black);

	sf::RectangleShape rectangle(sf::Vector2f(GameData::TILE_SIZE, GameData::TILE_SIZE));

	for (u64 y = 0; y < data->height; ++y) {
		for (u64 x = 0; x < data->width; ++x) {
			rectangle.setPosition(x * GameData::TILE_SIZE, y * GameData::TILE_SIZE);

			switch (data->getTile(x, y)) {
				case GameData::SNAKE_HEAD:
					rectangle.setFillColor(sf::Color::Magenta);
					window->draw(rectangle);
					break;
				case GameData::SNAKE_BODY:
					rectangle.setFillColor(sf::Color::Blue);
					window->draw(rectangle);
					break;
				case GameData::SNAKE_TAIL:
					rectangle.setFillColor(sf::Color::Cyan);
					window->draw(rectangle);
					break;
				case GameData::FOOD:
					rectangle.setFillColor(sf::Color::Green);
					window->draw(rectangle);
					break;
			}
		}
	}

	window->display();
}

vector<Event> getEvents() {
	if (!window) {
		return vector<Event>();
	}

	vector<Event> events;
	sf::Event event;

	// Collect all the events
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				events.push_back(Event(Event::QUIT));
				break;

			case sf::Event::KeyPressed:
				// Handle key presses
				switch (event.key.code) {
					case sf::Keyboard::Up:
						events.push_back(Event(Event::UP));
						break;
					case sf::Keyboard::Down:
						events.push_back(Event(Event::DOWN));
						break;
					case sf::Keyboard::Left:
						events.push_back(Event(Event::LEFT));
						break;
					case sf::Keyboard::Right:
						events.push_back(Event(Event::RIGHT));
						break;
					case sf::Keyboard::Num1:
						events.push_back(Event(Event::LIB1));
						break;
					case sf::Keyboard::Num2:
						events.push_back(Event(Event::LIB2));
						break;
					case sf::Keyboard::Num3:
						events.push_back(Event(Event::LIB3));
						break;
					case sf::Keyboard::Escape:
						events.push_back(Event(Event::QUIT));
						break;
				}
				break;
		}
	}

	return events;
}

void closeWindow() {
	if (!window) {
		return ;
	}

	window->close();
	delete window;
	window = NULL;
}

#ifdef __cplusplus
}
#endif