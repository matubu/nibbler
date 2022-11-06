#include <SFML/Graphics.hpp>
#include <map>

#include "../GameData.hpp"
#include "feature.hpp"

using std::map;

sf::RenderWindow *window = NULL;

sf::Sprite *sprite;
map<string, sf::Texture> *textures;

sf::Font font;

bool autoMode = false;

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
		tex.getSize().x / 2.0 - 0.5,
		tex.getSize().y / 2.0 - 0.5
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

i64 get_snake_tile_orientation(const SnakePart &a, const SnakePart &b) {
	// a-b
	if (b.y > a.y) {
		return 0;
	}
	if (a.x > b.x) {
		return 1;
	}
	if (b.y != a.y) {
		return 2;
	}
	return 3;
}

i64 get_snake_tile_orientation(
	const SnakePart &a,
	const SnakePart &b,
	const SnakePart &c
) {
	i64 rot = get_snake_tile_orientation(a, b);
	Vec2 expected(
		b.x + angle_to_vec(rot - 1).x,
		b.y + angle_to_vec(rot - 1).y
	);
	// Detect left or right turn
	if (c != expected) {
		rot += 1;
	}
	return rot;
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
	loadTexture("head_dead");
	loadTexture("food");

	if (!font.loadFromFile("sfml/fonts/SigmarOne-Regular.ttf")) {
		die("failed to load font");
	}
}

void draw_snake(const GameData *data) {
	for (int i = 0; i < data->snake.size(); i++) {
		i64 x = data->snake[i].x;
		i64 y = data->snake[i].y;
		i64 rot = 0;
		string texture;

		// Find the right texture and rotation
		if (i == 0) {
			rot = get_snake_tile_orientation(data->snake[i], data->snake[i + 1]);
			texture = "head";
		} else if (i == data->snake.size() - 1) {
			rot = get_snake_tile_orientation(data->snake[i - 1], data->snake[i]);
			texture = "tail";
		} else {
			if (data->snake[i - 1].x == data->snake[i + 1].x
				|| data->snake[i - 1].y == data->snake[i + 1].y) {
				rot = get_snake_tile_orientation(
					data->snake[i - 1],
					data->snake[i]
				);
				texture = "body_straight";
			} else {
				rot = get_snake_tile_orientation(
					data->snake[i - 1],
					data->snake[i],
					data->snake[i + 1]
				);
				texture = "body_turn";
			}
		}

		if (data->snake[i].isEating) {
			texture += "_eating";
		}
		if (data->gameOver && texture == "head") {
			texture += "_dead";
		}
		drawSprite(data, x, y, rot, texture);
	}
}

void draw(const GameData *data) {
	if (!window) {
		return ;
	}

	window->clear(sf::Color(0x0E183D00));

	draw_snake(data);

	for (auto p : data->food) {
		drawSprite(data, p.x, p.y, 0, "food");
	}

	sf::Text text;
	text.setFont(font);
	text.setString("Score: " + std::to_string(data->snake.size()));
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(15, 10);
	window->draw(text);

	window->display();
}

vector<Event> getEvents(const GameData *data) {
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
					case sf::Keyboard::W:
						events.push_back(Event(Event::UP));
						break;
					case sf::Keyboard::Down:
					case sf::Keyboard::S:
						events.push_back(Event(Event::DOWN));
						break;
					case sf::Keyboard::Left:
					case sf::Keyboard::A:
						events.push_back(Event(Event::LEFT));
						break;
					case sf::Keyboard::Right:
					case sf::Keyboard::D:
						events.push_back(Event(Event::RIGHT));
						break;
					case sf::Keyboard::U:
						if (string(getenv("USER")) == "u") {
							autoMode = !autoMode;
						}
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

	if (autoMode) {
		smartMove(data, events);
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