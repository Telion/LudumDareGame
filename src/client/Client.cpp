#include "Client.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <emscripten.h>

Client::Client()
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	characterSprites = UniformSpriteSheet(renderer, "../../src/client/resources/samurai.png", 96, 96);
	terrainSprites = UniformSpriteSheet(renderer, "../../src/client/resources/terrainSprites.png", 64, 64);

	lastTime = emscripten_get_now() * 1000;

	chunk = Chunk(0, 0, "../../src/server/maps/0,0.tmx");
}

Client::~Client()
{
	IMG_Quit();
	SDL_Quit();
}

void Client::gameLoop()
{
	unsigned time = emscripten_get_now() * 1000;
	int delta = time - lastTime;
	lastTime = time;

	if (delta < 0)
		delta = 0;

	if (delta > 300000)
		delta = 300000;

	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(nullptr);

	tick(delta);
	render();
}

void Client::tick(int microseconds)
{
	character.tick(microseconds, keys);
}

void Client::render()
{
	Position base;
	base.x = character.getPosition().x - (screenWidth - 96) / 2;
	base.y = character.getPosition().y - (screenHeight - 96) / 2;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, nullptr);

	chunk.render(renderer, terrainSprites, base);
	character.render(renderer, characterSprites, base);

	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
}
