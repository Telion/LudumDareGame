#include "ClientCharacter.h"

#include "UniformSpriteSheet.h"

#include <SDL/SDL.h>

void ClientCharacter::tick(int microseconds, unsigned char* keys)
{
	bool left = keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT];
	bool right = !left && (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);
	bool up = keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP];
	bool down = !up && (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]);

	int numDirections = (int)left + (int)right + (int)up + (int)down;

	double speed = this->speed * microseconds / 1000000;
	if (numDirections == 2)
		speed /= 1.41421356237;

	position.x = position.x - left * speed + right * speed;
	position.y = position.y - up * speed + down * speed;
}

void ClientCharacter::render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, Position base)
{
	spriteSheet.renderSprite(0, position, base, renderer);
}

Position ClientCharacter::getPosition() const
{
	return position;
}