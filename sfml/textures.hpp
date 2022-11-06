#pragma once

#include <SFML/Graphics.hpp>
#include <map>

using std::map;

map<string, sf::Texture> *textures;
vector<string> texturesPacks{"blue", "orange", "green"};
u64 currentTexturePack = 0;

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
	loadTexture("food");
	loadTexture("death_overlay");
}