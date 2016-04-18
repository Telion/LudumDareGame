#include "Chunk.h"

#include "Tile.h"
#include "VectorMath.h"
#include "../client/UniformSpriteSheet.h"
#include "MyAssert.h"

#include <SDL2/SDL.h>
#include <cstdio>
#include <sstream>

Chunk::Chunk()
{

}

Chunk::Chunk(int x, int y, const std::vector<Tile>& tiles)
{
	this->x = x;
	this->y = y;
	this->tiles = tiles;
}

void Chunk::markLoading()
{
	loading = true;
}

Tile Chunk::getTile(int x, int y) const
{
	MyAssert(x >= 0);
	MyAssert(y >= 0);
	MyAssert(x < chunkWidth);
	MyAssert(y < chunkHeight);

	return tiles[y * chunkWidth + x];
}

void Chunk::render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, Vector2 base, int screenWidth, int screenHeight)
{
	//if (!texture)
	//{
	//	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, chunkWidth * spriteSheet.getSpriteWidth(), chunkHeight * spriteSheet.getSpriteHeight());
	//	SDL_SetRenderTarget(renderer, texture);

	//	for (std::size_t i = 0; i < tiles.size(); ++i)
	//	{
	//		int tileX = i % chunkWidth;
	//		int tileY = i / chunkWidth;

	//		Vector2 position;
	//		position.x = (this->x * chunkWidth + tileX) * spriteSheet.getSpriteWidth();
	//		position.y = (this->y * chunkHeight + tileY) * spriteSheet.getSpriteHeight();

	//		if (tiles[i].id != 0)
	//			spriteSheet.renderSprite(tiles[i].id - firstgid, position, base, renderer);
	//	}

	//	SDL_SetRenderTarget(renderer, nullptr);
	//}

	//SDL_Rect dest;
	//dest.x = this->x * chunkWidth - base.x;
	//dest.y = this->y * chunkHeight - base.y;
	//dest.w = chunkWidth * spriteSheet.getSpriteWidth();
	//dest.h = chunkHeight * spriteSheet.getSpriteHeight();

	//SDL_RenderCopy(renderer, texture, nullptr, &dest);

	int spriteWidth = spriteSheet.getSpriteWidth();
	int spriteHeight = spriteSheet.getSpriteHeight();

	int x = this->x * chunkWidth * spriteWidth;
	int y = this->y * chunkHeight * spriteHeight;

	for (std::size_t i = 0; i < tiles.size(); ++i)
	{
		int tileX = i % chunkWidth * spriteWidth;
		int tileY = i / chunkWidth * spriteHeight;

		if (x + tileX > base.x + screenWidth || y + tileY > base.y + screenHeight || x + tileX + spriteWidth < base.x || y + tileY + spriteHeight < base.y)
			continue;

		Vector2 position;
		position.x = x + tileX;
		position.y = y + tileY;

		if (tiles[i].id != -1)
			spriteSheet.renderSprite(tiles[i].id, position, base, renderer);
	}
}

bool Chunk::isValid() const
{
	return tiles.size() > 0;
}

const std::vector<Tile>& Chunk::getTiles() const
{
	return tiles;
}

bool Chunk::isLoading() const
{
	return loading;
}