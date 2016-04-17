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
		position.x -= chunkWidth * 64 + 1;
	if (position.y < 0)
		position.y -= chunkHeight * 64 + 1;

	p.first = (int)floor(position.x) / (chunkWidth * 64);
	p.second = (int)floor(position.y) / (chunkHeight * 64);

	return p;
}