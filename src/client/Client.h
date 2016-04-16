#pragma once

#include "ClientCharacter.h"
#include "UniformSpriteSheet.h"
#include "../common/Chunk.h"

#include <SDL/SDL.h>

class Client
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	unsigned char* keys;

	unsigned lastTime;

	UniformSpriteSheet characterSprites;
	UniformSpriteSheet terrainSprites;

	ClientCharacter character;
	Chunk chunk;

public:
	Client();

	~Client();
	
	void gameLoop();

	void tick(int microseconds);
	void render();

	static const int screenWidth = 960;
	static const int screenHeight = 720;
};