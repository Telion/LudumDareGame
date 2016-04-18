#include "CollisionResolution.h"

#include "World.h"

Vector2 resolveCollisions(Rect start, Rect end, const World& world)
{
	const double epsilon = .01;

	Vector2 result(end.x, end.y);

	int currentTileX = floor(start.x / 64), currentTileXRight = floor((start.x + start.w) / 64), currentTileY = floor(start.y / 64), currentTileYBottom = floor((start.y + start.h) / 64);
	int newTileX = floor(end.x / 64), newTileXRight = floor((end.x + end.w) / 64), newTileY = floor(end.y / 64), newTileYBottom = floor((end.y + end.h) / 64);

	if (end.x < start.x)
	{
		for (int x = currentTileX; x >= newTileX; --x)
			for (int y = currentTileY; y <= currentTileYBottom; ++y)
			{
				if (world.getTile(x, y).isSolid())
				{
					result.x = fmin((x + 1) * 64 + epsilon, start.x);
					goto end_left;
				}
			}
	end_left:;
	}

	if (end.x + end.w > start.x + start.w)
	{
		for (int x = currentTileXRight; x <= newTileXRight; ++x)
			for (int y = currentTileY; y <= currentTileYBottom; ++y)
			{
				if (world.getTile(x, y).isSolid())
				{
					result.x = fmax(x * 64 - end.w - epsilon, start.x);
					goto end_right;
				}
			}
	end_right:;
	}

	currentTileX = floor(start.x / 64); currentTileXRight = floor((start.x + start.w) / 64);
	newTileX = floor(result.x / 64); newTileXRight = floor((result.x + start.w) / 64);

	if (end.y < start.y)
	{
		for (int y = currentTileY; y >= newTileY; --y)
			for (int x = currentTileX; x <= currentTileXRight; ++x)
			{
				if (world.getTile(x, y).isSolid())
				{
					result.y = fmin((y + 1) * 64 + epsilon, start.y);
					goto end_up;
				}
			}
	end_up:;
	}

	if (end.y + end.h > start.y + start.h)
	{
		for (int y = currentTileYBottom; y <= newTileYBottom; ++y)
			for (int x = currentTileX; x <= currentTileXRight; ++x)
			{
				if (world.getTile(x, y).isSolid())
				{
					result.y = fmax(y * 64 - end.h - epsilon, start.y);
					goto end_down;
				}
			}
	end_down:;
	}

	return result;
}