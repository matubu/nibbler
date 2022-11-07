#include <SFML/Graphics.hpp>

#include "../GameData.hpp"
#include "textures.hpp"

sf::RenderWindow *window = NULL;

sf::Sprite *sprite;

sf::Font font;

bool rainbowMode = false;

const u64 HELP_FONT_SIZE = 15;
const u64 TEXT_FONT_SIZE = 24;
const u64 TITLE_FONT_SIZE = 48;

void drawSprite(const GameData *data, i64 x, i64 y, i64 rot, const string &path) {
	const sf::Texture &tex = getTexture(path);
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
		"Nibbler - SFML",
		sf::Style::Close
	);

	window->setVerticalSyncEnabled(true);

	sprite = new sf::Sprite();
	textures = new map<string, sf::Texture>();
	loadAllTextures();

	if (!font.loadFromFile("fonts/SigmarOne-Regular.ttf")) {
		die("failed to load font");
	}
}

void draw_snake(const GameData *data, const Snake &snake) {
	for (u64 i = 0; i < snake.size(); i++) {
		auto &part = snake[i];
		i64 rot = 0;
		string texture;

		// Find the right texture and rotation
		if (i == 0) {
			rot = get_snake_tile_orientation(part, snake[i + 1]);
			texture = "head";
		} else if (i == snake.size() - 1) {
			rot = get_snake_tile_orientation(snake[i - 1], part);
			texture = "tail";
		} else {
			if (snake[i - 1].x == snake[i + 1].x
				|| snake[i - 1].y == snake[i + 1].y) {
				rot = get_snake_tile_orientation(
					snake[i - 1],
					part
				);
				texture = "body_straight";
			} else {
				rot = get_snake_tile_orientation(
					snake[i - 1],
					part,
					snake[i + 1]
				);
				texture = "body_turn";
			}
		}

		if (snake.isDead && texture == "head") {
			texture += "_dead";
		}
		else if (part.isEating) {
			texture += "_eating";
		}
		texture = texturesPacks[(rainbowMode ? i : snake.score) % texturesPacks.size()] + "/" + texture;
		drawSprite(data, part.x, part.y, rot, texture);
	}
}

void draw(const GameData *data) {
	if (!window) {
		return ;
	}

	window->clear(sf::Color(0x0E183D00));

	for (auto &snake : data->snakes) {
		draw_snake(data, snake);
	}

	drawSprite(data, data->food.x, data->food.y, 0,
			getFoodTexture(data->food.x, data->food.y));

	sf::Text text;
	text.setFont(font);
	text.setString(data->getScoreText()
		+ "\n[+/-] Speed: " + std::to_string(data->speed)
	);
	text.setCharacterSize(HELP_FONT_SIZE);
	text.setFillColor(sf::Color::White);
	text.setPosition(15, 10);
	window->draw(text);

	if (data->gameOver) {
		const sf::Texture &deathOverlayTexture = getTexture("death_overlay");
		sf::Sprite deathOverlay(deathOverlayTexture);
		deathOverlay.setScale(sf::Vector2f(
			((float)window->getSize().x / deathOverlayTexture.getSize().x),
			((float)window->getSize().y / deathOverlayTexture.getSize().y)
		));
		window->draw(deathOverlay);

		text.setString("Game Over!");
		text.setCharacterSize(TITLE_FONT_SIZE);
		text.setPosition(
			window->getSize().x / 2 - text.getGlobalBounds().width / 2,
			window->getSize().y / 2 - TITLE_FONT_SIZE - TEXT_FONT_SIZE
		);
		window->draw(text);

		text.setString("Press R to Restart");
		text.setCharacterSize(TEXT_FONT_SIZE);
		text.setPosition(
			window->getSize().x / 2 - text.getGlobalBounds().width / 2,
			window->getSize().y / 2 + TEXT_FONT_SIZE / 2
		);
		window->draw(text);
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
					case sf::Keyboard::Left:
						events.push_back(Event(Event::LEFT));
						break;
					case sf::Keyboard::Down:
						events.push_back(Event(Event::DOWN));
						break;
					case sf::Keyboard::Right:
						events.push_back(Event(Event::RIGHT));
						break;
					case sf::Keyboard::W:
						events.push_back(Event(Event::W));
						break;
					case sf::Keyboard::A:
						events.push_back(Event(Event::A));
						break;
					case sf::Keyboard::S:
						events.push_back(Event(Event::S));
						break;
					case sf::Keyboard::D:
						events.push_back(Event(Event::D));
						break;
					case sf::Keyboard::Space:
						rainbowMode = !rainbowMode;
						break;
					case sf::Keyboard::Add:
						events.push_back(Event(Event::SPEED_UP));
						break;
					case sf::Keyboard::Subtract:
						events.push_back(Event(Event::SPEED_DOWN));
						break;
					case sf::Keyboard::R:
						events.push_back(Event(Event::RESET));
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
					default:
						break;
				}
				default:
					break;
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