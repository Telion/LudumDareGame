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

	bool loading;

public:
	Chunk();
	Chunk(int x, int y, const std::vector<Tile>& tiles);

	void markLoading();

	Tile getTile(int x, int y) const;

	void render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, Vector2 base, int screenWidth, int screenHeight);

	bool isValid() const;
	const std::vector<Tile>& getTiles() const;
	bool isLoading() const;

	static const int chunkWidth = 64;
	static const int chunkHeight = 64;
};