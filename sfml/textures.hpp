#pragma once

#include <SFML/Graphics.hpp>
#include <map>

using std::map;

map<string, sf::Texture> *textures;
vector<string> texturesPacks{"blue", "orange", "green"};

u64 nbFoodVariants = 5;

void loadTexture(const string &name) {
	string path = "assets/" + name + ".png";

	if (!(*textures)[name].loadFromFile(path.c_str())) {
		die("failed to load texture");
	}
}

void loadTextureFromPack(const string &name) {
	for (auto &pack : texturesPacks) {
		loadTexture(pack + "/" + name);
	}
}

const sf::Texture &getTexture(const string &name) {
	return textures->find(name)->second;
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