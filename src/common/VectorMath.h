#pragma once

#include <utility>
#include <cmath>

struct Vector2
{
	double x;
	double y;

	Vector2():
		x(0), y(0)
	{

	}

	Vector2(double x, double y):
		x(x), y(y)
	{

	}
};

struct Rect
{
	double x;
	double y;
	double w;
	double h;

	Rect():
		x(0), y(0), w(0), h(0)
	{

	}

	Rect(double x, double y, double w, double h) :
		x(x), y(y), w(w), h(h)
	{

	}
};

inline double magnitude(Vector2 v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

inline Vector2 normalize(Vector2 v)
{
	double length = magnitude(v);
	Vector2 result;
	result.x = v.x / length;
	result.y = v.y / length;
	return result;
}

inline Vector2 operator +(Vector2 a, Vector2 b)
{
	Vector2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

inline Vector2 operator -(Vector2 a, Vector2 b)
{
	Vector2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}

inline Vector2 operator *(Vector2 v, double s)
{
	Vector2 result;
	result.x = v.x * s;
	result.y = v.y * s;
	return result;
}

inline Vector2 operator /(Vector2 v, double s)
{
	Vector2 result;
	result.x = v.x / s;
	result.y = v.y / s;
	return result;
}

inline std::pair<int, int> positionToChunkCoordinates(Vector2 position, int chunkWidth, int chunkHeight)
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

inline Vector2 lerp(Vector2 a, Vector2 b, double fractionB)
{
	Vector2 p;
	p.x = a.x * (1 - fractionB) + b.x * fractionB;
	p.y = a.y * (1 - fractionB) + b.y * fractionB;

	return p;
}