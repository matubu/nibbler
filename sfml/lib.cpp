#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

#include "../GameData.hpp"

using std::map;

sf::RenderWindow *window = NULL;

sf::Sprite *sprite;
map<string, sf::Texture> *textures;

void loadTexture(const string &name) {
	string path = "sfml/assets/" + name + ".png";

	if (!(*textures)[name].loadFromFile(path.c_str())) {
		die("failed to load texture");
	}
}

void drawSprite(const GameData *data, i64 x, i64 y, i64 rot, const string &path) {
	sf::Texture &tex = (*textures)[path];
	sprite->setTexture(tex);
	sprite->setOrigin(
		tex.getSize().x / 2,
		tex.getSize().y / 2
	);
	sprite->setScale(
		((float)window->getSize().x / tex.getSize().x) / data->width,
		((float)window->getSize().y / tex.getSize().y) / data->height
	);
	sprite->setPosition(
		x * GameData::TILE_SIZE + GameData::TILE_SIZE / 2,
		y * GameData::TILE_SIZE + GameData::TILE_SIZE / 2
	);
	sprite->setRotation(rot * 90);
	window->draw(*sprite);
}

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

	sprite = new sf::Sprite();
	textures = new map<string, sf::Texture>();
	loadTexture("head");
	loadTexture("body_straight");
	loadTexture("body_turn");
	loadTexture("tail");
	loadTexture("head_eating");
	loadTexture("body_straight_eating");
	loadTexture("body_turn_eating");
	loadTexture("tail_eating");
	loadTexture("food");
}

i64 atan_4(i64 x, i64 y) {
	if (y < 0) {
		return 0;
	}
	if (x > 0) {
		return 1;
	}
	if (y > 0) {
		return 2;
	}
	return 3;
}

Vec2 angle_to_vec(i64 angle) {
	switch ((angle % 4 + 4) % 4) {
		case 0:
			return {0, -1};
		case 1:
			return {1, 0};
		case 2:
			return {0, 1};
		case 3:
			return {-1, 0};
	}
	return {0, 0};
}

void draw(const GameData *data) {
	if (!window) {
		return ;
	}

	window->clear(sf::Color(0x0E183D00));

	for (int i = 0; i < data->snake.size(); i++) {
		i64 x = data->snake[i].x;
		i64 y = data->snake[i].y;
		i64 rot = 0;
		string texture;

		// Find the right texture and rotation
		if (i == 0) {
			rot = atan_4(
				x - data->snake[i + 1].x,
				y - data->snake[i + 1].y
			);
			texture = "head";
		} else if (i == data->snake.size() - 1) {
			rot = atan_4(
				data->snake[i - 1].x - x,
				data->snake[i - 1].y - y
			);
			texture = "tail";
		} else {
			if (data->snake[i - 1].x == data->snake[i + 1].x
				|| data->snake[i - 1].y == data->snake[i + 1].y) {
				if (data->snake[i - 1].y == y) {
					rot = 1;
				}
				texture = "body_straight";
			} else {
				rot = atan_4(
					data->snake[i - 1].x - x,
					data->snake[i - 1].y - y	
				);
				Vec2 expected(
					x + angle_to_vec(rot - 1).x,
					y + angle_to_vec(rot - 1).y
				);
				// Detect left or right turn
				if (data->snake[i + 1] != expected) {
					rot += 1;
				}
				texture = "body_turn";
			}
		}

		if (data->snake[i].isEating) {
			texture += "_eating";
		}
		drawSprite(data, x, y, rot, texture);
	}

	for (auto p : data->food) {
		drawSprite(data, p.x, p.y, 0, "food");
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

	delete sprite;
	delete textures;
}

#ifdef __cplusplus
}
#endif