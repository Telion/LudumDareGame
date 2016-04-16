#include "Client.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <emscripten.h>

Client::Client()
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 720, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	characterSprites = UniformSpriteSheet(renderer, "../../src/client/resources/samurai.png", 96, 96);

	lastTime = emscripten_get_now() * 1000;
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, nullptr);

	character.render(renderer, characterSprites, Position());

	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
}
