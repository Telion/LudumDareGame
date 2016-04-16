#pragma once

#include "../common/VectorMath.h"

#include <string>
#include <SDL/SDL.h>

class UniformSpriteSheet
{
	SDL_Surface* sheet;
	SDL_Texture* texture;
	int spriteWidth;
	int spriteHeight;

public:
	UniformSpriteSheet();
	UniformSpriteSheet(SDL_Renderer* renderer, const std::string& file, int spriteWidth, int spriteHeight);

	void renderSprite(unsigned sprite, Position position, Position base, SDL_Renderer* renderer) const;

	int getSpriteWidth() const
	{
		return spriteWidth;
	}

	int getSpriteHeight() const
	{
		return spriteHeight;
	}
};