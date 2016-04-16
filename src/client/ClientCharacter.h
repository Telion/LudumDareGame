#pragma once

#include "../common/VectorMath.h"
#include "UniformSpriteSheet.h"

#include <SDL/SDL.h>

class ClientCharacter
{
	Position position;

	double speed = 150.0;

public:
	void tick(int microseconds, unsigned char* keys);

	void render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, Position base);

	Position getPosition() const;
};
