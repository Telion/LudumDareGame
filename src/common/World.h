#pragma once

#include "Chunk.h"
#include "Tile.h"
#include "Socket.h"
#include "../client/UniformSpriteSheet.h"

#include <SDL2/SDL.h>
#include <map>
#include <memory>

class Chunk;

class World
{
	std::map<std::pair<int, int>, Chunk> chunks;

public:
	bool hasChunk(int x, int y) const;

	Chunk& getChunk(int x, int y);
	const Chunk& getChunk(int x, int y) const;
	void setChunk(int x, int y, const Chunk& chunk);

	Tile getTile(int x, int y) const;

	void updateLoadedChunks(Vector2 playerPosition, const std::shared_ptr<Socket>& socket);

	void render(SDL_Renderer* renderer, const UniformSpriteSheet& terrainSprites, Vector2 base, int screenWidth, int screenHeight) const;
};