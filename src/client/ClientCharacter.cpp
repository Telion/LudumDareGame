#include "ClientCharacter.h"

#include "UniformSpriteSheet.h"
#include "../common/Chunk.h"
#include "../common/Tile.h"
#include "../common/World.h"
#include "../common/CollisionResolution.h"
#include "../common/Config.h"

#include <SDL/SDL.h>
#include <map>
#include <utility>
#include <cmath>

ClientCharacter::ClientCharacter():
	position(64 * 32, 64 * 45)
{

}

void ClientCharacter::tick(unsigned time, int delta, const unsigned char* keys, bool mouseDown, int mouseX, int mouseY, const World& world)
{
	Action action;

	lastPosition = position;

	if (mouseDown && time - lastShotTime >= shotDelay)
	{
		lastShotTime = time;

		Vector2 shotDirection = Vector2(mouseX, mouseY) - Vector2(screenWidth / 2, screenHeight / 2);

		if (magnitude(shotDirection) > .5)
		{
			action.shooting = true;
			action.shotDirection = normalize(shotDirection);
		}
	}

	if (time - lastShotTime >= shotDelay)
	{
		bool left = keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT];
		bool right = !left && (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);
		bool up = keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP];
		bool down = !up && (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]);

		if (left || right || up || down)
		{
			Vector2 movement((int)right - (int)left, (int)down - (int)up);
			movement = normalize(movement) * this->speed * delta / 1000000;

			Rect oldPos(position.x, position.y, width, height);
			Rect newPos(position.x + movement.x, position.y + movement.y, width, height);

			position = resolveCollisions(oldPos, newPos, world);
		}
	}

	action.time = time;
	action.position = position;
	actionLog.push_back(action);
}

void ClientCharacter::render(SDL_Renderer* renderer, const UniformSpriteSheet& spriteSheet, int sprite, Vector2 base, double fractionNew)
{
	Vector2 p = lerp(lastPosition, position, fractionNew);

	spriteSheet.renderSprite(sprite, p, base, 0, renderer);
}

Vector2 ClientCharacter::getPosition() const
{
	return position;
}