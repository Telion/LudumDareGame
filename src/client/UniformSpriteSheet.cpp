#include "UniformSpriteSheet.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <cassert>

UniformSpriteSheet::UniformSpriteSheet()
{

}

UniformSpriteSheet::UniformSpriteSheet(SDL_Renderer* renderer, const std::string& file, int spriteWidth, int spriteHeight)
{
	sheet = IMG_Load(file.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, sheet);
	this->spriteWidth = spriteWidth;
	this->spriteHeight = spriteHeight;
}

void UniformSpriteSheet::renderSprite(unsigned sprite, Position position, Position base, SDL_Renderer* renderer) const
{
	int sheetWidth = sheet->w, sheetHeight = sheet->h;
	int spritesPerRow = sheetWidth / spriteWidth;
	int spritesPerCol = sheetHeight / spriteHeight;

	int row = sprite % spritesPerRow;
	int col = sprite / spritesPerRow;

	assert(col < spritesPerCol);

	SDL_Rect src;
	src.x = row * spriteWidth;
	src.y = col * spriteHeight;
	src.w = spriteWidth;
	src.h = spriteHeight;

	SDL_Rect dst;
	dst.x = position.x - base.x;
	dst.y = position.y - base.y;
	dst.w = spriteWidth;
	dst.h = spriteHeight;

	SDL_RenderCopy(renderer, texture, &src, &dst);
	//SDL_BlitSurface(sheet, &src, destination, &dst);
}