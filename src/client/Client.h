#pragma once

#include "ClientCharacter.h"
#include "UniformSpriteSheet.h"

#include <SDL/SDL.h>

class Client
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	unsigned char* keys;

	unsigned lastTime;

	UniformSpriteSheet characterSprites;

	ClientCharacter character;

public:
	Client();

	~Client();
	
	void gameLoop();

	void tick(int microseconds);
	void render();
};