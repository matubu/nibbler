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

	for (int i = 0; i < data->snake.size(); i++) {
		rectangle.setPosition(
			data->snake[i].first * GameData::TILE_SIZE,
			data->snake[i].second * GameData::TILE_SIZE
		);
		if (i == 0) {
			rectangle.setFillColor(sf::Color::Magenta);
		} else if (i == data->snake.size() - 1) {
			rectangle.setFillColor(sf::Color::Cyan);
		} else {
			rectangle.setFillColor(sf::Color::Blue);
		}
		window->draw(rectangle);
	}

	sf::CircleShape circle(GameData::TILE_SIZE / 2);
	for (auto food : data->food) {
		circle.setPosition(
			food.first * GameData::TILE_SIZE,
			food.second * GameData::TILE_SIZE
		);
		circle.setFillColor(sf::Color::Red);
		window->draw(circle);
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