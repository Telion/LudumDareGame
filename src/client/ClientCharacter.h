#pragma once

#include "../common/World.h"
#include "../common/VectorMath.h"
#include "UniformSpriteSheet.h"

#include <SDL/SDL.h>
#include <map>

class ClientCharacter
{
	Vector2 position;
	Vector2 lastPosition;

	double speed = 200.0;
	int width = 96;
	int height = 96;

public:
	long long id;

	ClientCharacter();

	void tick(int microseconds, const unsigned char* keys, const World& world);

	void render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, int sprite, Vector2 base, double fractionNew);

	Vector2 getPosition() const;
};
