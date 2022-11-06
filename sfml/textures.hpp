#pragma once

#include <SFML/Graphics.hpp>
#include <map>

using std::map;

map<string, sf::Texture> *textures;
vector<string> texturesPacks{"blue", "orange", "green"};
u64 currentTexturePack = 0;

u64 nbFoodVariants = 5;

void loadTexture(const string &name) {
	string path = "sfml/assets/" + name + ".png";

	if (!(*textures)[name].loadFromFile(path.c_str())) {
		die("failed to load texture");
	}
}

void loadTextureFromPack(const string &name) {
	string path = "sfml/assets/" + texturesPacks[currentTexturePack] + "/" + name + ".png";

	if (!(*textures)[name].loadFromFile(path.c_str())) {
		die("failed to load texture");
	}
}

const string getFoodTexture(i64 x, i64 y) {
	srand(x << 16 | y);
	return "food-" + std::to_string(rand() % nbFoodVariants);
}

void loadAllTextures() {
	loadTextureFromPack("head");
	loadTextureFromPack("body_straight");
	loadTextureFromPack("body_turn");
	loadTextureFromPack("tail");
	loadTextureFromPack("head_eating");
	loadTextureFromPack("body_straight_eating");
	loadTextureFromPack("body_turn_eating");
	loadTextureFromPack("tail_eating");
	loadTextureFromPack("head_dead");
	for (u64 i = 0; i < nbFoodVariants; i++) {
		loadTexture("food-" + std::to_string(i));
	}
	loadTexture("death_overlay");
}