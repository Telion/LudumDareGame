#pragma once

#include "Tile.h"
#include "VectorMath.h"

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class UniformSpriteSheet;

class Chunk
{
	std::vector<Tile> tiles;
	int x;
	int y;
	int firstgid;

public:
	Chunk();
	Chunk(int x, int y, const std::string& filename);

	void render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, Position base);

	static const int chunkWidth = 64;
	static const int chunkHeight = 64;
};