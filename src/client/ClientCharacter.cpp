#include "ClientCharacter.h"

#include "UniformSpriteSheet.h"
#include "../common/Chunk.h"
#include "../common/Tile.h"
#include "../common/World.h"

#include <SDL/SDL.h>
#include <map>
#include <utility>
#include <cmath>

const double epsilon = .01;

ClientCharacter::ClientCharacter()
{

}

void ClientCharacter::tick(int microseconds, const unsigned char* keys, const World& world)
{
	lastPosition = position;

	bool left = keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT];
	bool right = !left && (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);
	bool up = keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP];
	bool down = !up && (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]);

	int numDirections = (int)left + (int)right + (int)up + (int)down;

	double speed = this->speed * microseconds / 1000000;
	if (numDirections == 2)
		speed /= 1.41421356237;

	double newX = position.x - left * speed + right * speed;
	double newY = position.y - up * speed + down * speed;

	int currentTileX = floor(position.x / 64), currentTileXRight = floor((position.x + width) / 64), currentTileY = floor(position.y / 64), currentTileYBottom = floor((position.y + height) / 64);
	int newTileX = floor(newX / 64), newTileXRight = floor((newX + width) / 64), newTileY = floor(newY / 64), newTileYBottom = floor((newY + height) / 64);

	if (left)
	{
		for (int x = currentTileX; x >= newTileX; --x)
			for (int y = currentTileY; y <= currentTileYBottom; ++y)
			{
				if (world.getTile(x, y).isSolid())
				{
					newX = fmin((x + 1) * 64 + epsilon, position.x);
					goto end_left;
				}
			}
	end_left:;
	}

	if (right)
	{
		for (int x = currentTileXRight; x <= newTileXRight; ++x)
			for (int y = currentTileY; y <= currentTileYBottom; ++y)
			{
				if (world.getTile(x, y).isSolid())
				{
					newX = fmax(x * 64 - width - epsilon, position.x);
					goto end_right;
				}
			}
	end_right:;
	}

	currentTileX = floor(position.x / 64); currentTileXRight = floor((position.x + width) / 64);
	newTileX = floor(newX / 64); newTileXRight = floor((newX + width) / 64);
	
	if (up)
	{
		for (int y = currentTileY; y >= newTileY; --y)
			for (int x = currentTileX; x <= currentTileXRight; ++x)
			{
				if (world.getTile(x, y).isSolid())
				{
					newY = fmin((y + 1) * 64 + epsilon, position.y);
					goto end_up;
				}
			}
	end_up:;
	}

	if (down)
	{
		for (int y = currentTileYBottom; y <= newTileYBottom; ++y)
			for (int x = currentTileX; x <= currentTileXRight; ++x)
			{
				if (world.getTile(x, y).isSolid())
				{
					newY = fmax(y * 64 - height - epsilon, position.y);
					goto end_down;
				}
			}
	end_down:;
	}

	position.x = newX;
	position.y = newY;
}

void ClientCharacter::render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, Position base, double fractionNew)
{
	Position p = lerp(lastPosition, position, fractionNew);

	spriteSheet.renderSprite(0, p, base, renderer);
}

Position ClientCharacter::getPosition() const
{
	return position;
}