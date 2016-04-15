#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

extern "C" int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

	SDL_Surface* image = IMG_Load("../../src/client/resources/test.png");
	SDL_BlitSurface(image, nullptr, windowSurface, nullptr);

	SDL_UpdateWindowSurface(window);

	IMG_Quit();
	SDL_Quit();

	return 0;
}
