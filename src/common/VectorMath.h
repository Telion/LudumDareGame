#pragma once

#include <utility>
#include <cmath>

struct Position
{
	double x;
	double y;
};

inline std::pair<int, int> positionToChunkCoordinates(Position position, int chunkWidth, int chunkHeight)
{
	std::pair<int, int> p;
	
	if (position.x < 0)
		position.x -= chunkWidth * 64 - 1;
	if (position.y < 0)
		position.y -= chunkHeight * 64 - 1;

	p.first = (int)floor(position.x) / (chunkWidth * 64);
	p.second = (int)floor(position.y) / (chunkHeight * 64);

	return p;
}

inline std::pair<int, int> tileToChunkCoordinates(int tileX, int tileY, int chunkWidth, int chunkHeight)
{
	if (tileX < 0)
		tileX -= chunkWidth - 1;
	if (tileY < 0)
		tileY -= chunkHeight - 1;

	return std::make_pair(tileX / chunkWidth, tileY / chunkHeight);
}