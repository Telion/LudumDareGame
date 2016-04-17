#pragma once

#include "../common/World.h"
#include "../common/VectorMath.h"
#include "UniformSpriteSheet.h"

#include <SDL/SDL.h>
#include <map>

class ClientCharacter
{
	Position position;

	double speed = 200.0;
	int width = 96;
	int height = 96;

public:
	ClientCharacter();

	void tick(int microseconds, const unsigned char* keys, const World& world);

	void render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, Position base);

	Position getPosition() const;
};
