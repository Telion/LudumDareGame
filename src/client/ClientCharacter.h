#pragma once

#include "../common/Action.h"
#include "../common/World.h"
#include "../common/VectorMath.h"
#include "UniformSpriteSheet.h"

#include <SDL/SDL.h>
#include <map>
#include <vector>

class ClientCharacter
{
	Vector2 position;
	Vector2 lastPosition;

	double speed = 200.0;
	int width = 96;
	int height = 96;

	unsigned lastShotTime;
	static const int shotDelay = 600000;

public:
	long long id;
	std::vector<Action> actionLog;

	ClientCharacter();

	void tick(unsigned time, int delta, const unsigned char* keys, bool mouseDown, int mouseX, int mouseY, const World& world);

	void render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, int sprite, Vector2 base, double fractionNew);

	Vector2 getPosition() const;
};
