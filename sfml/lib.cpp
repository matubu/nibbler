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
		(x * Tile::TILE_SIZE + Tile::TILE_SIZE / 2) * 2,
		(y * Tile::TILE_SIZE + Tile::TILE_SIZE / 2) * 2
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
			data->width * Tile::TILE_SIZE * 2,
			data->height * Tile::TILE_SIZE * 2
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

i64 getEvents() {
	if (!window) {
		return Event::QUIT;
	}

	sf::Event event;

	// Collect all the events
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				return Event::QUIT;

			case sf::Event::KeyPressed:
				// Handle key presses
				switch (event.key.code) {
					case sf::Keyboard::Up:
						return Event::UP;
					case sf::Keyboard::Left:
						return Event::LEFT;
					case sf::Keyboard::Down:
						return Event::DOWN;
					case sf::Keyboard::Right:
						return Event::RIGHT;
					case sf::Keyboard::W:
						return Event::W;
					case sf::Keyboard::A:
						return Event::A;
					case sf::Keyboard::S:
						return Event::S;
					case sf::Keyboard::D:
						return Event::D;
					case sf::Keyboard::Space:
						rainbowMode = !rainbowMode;
					case sf::Keyboard::Add:
						return Event::SPEED_UP;
					case sf::Keyboard::Subtract:
						return Event::SPEED_DOWN;
					case sf::Keyboard::R:
						return Event::RESET;
					case sf::Keyboard::Num1:
						return Event::LIB1;
					case sf::Keyboard::Num2:
						return Event::LIB2;
					case sf::Keyboard::Num3:
						return Event::LIB3;
					case sf::Keyboard::Escape:
						return Event::QUIT;
					default:
						break ;
				}
				default:
					break;
				break;
		}
	}

	return -1;
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