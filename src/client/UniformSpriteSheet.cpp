#include "UniformSpriteSheet.h"
#include "../common/MyAssert.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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

	int col = sprite % spritesPerRow;
	int row = sprite / spritesPerRow;

	MyAssert(row < spritesPerCol);

	SDL_Rect src;
	src.x = col * spriteWidth;
	src.y = row * spriteHeight;
	src.w = spriteWidth;
	src.h = spriteHeight;

	SDL_Rect dst;
	dst.x = position.x - base.x;
	dst.y = position.y - base.y;
	dst.w = spriteWidth;
	dst.h = spriteHeight;

	SDL_RenderCopy(renderer, texture, &src, &dst);
}