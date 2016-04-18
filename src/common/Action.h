#pragma once

#include "VectorMath.h"

class Action
{
public:
	Action() :
		time(0), position(), shooting(false), shotDirection()
	{
		static long long nextId = 0;
		id = nextId++;
	}

	long long id;

	unsigned time;

	Vector2 position;

	bool shooting;
	Vector2 shotDirection;
};