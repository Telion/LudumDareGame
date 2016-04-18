#pragma once

#include "VectorMath.h"
#include "Action.h"

#include <vector>

struct CommonEntity
{
	enum class Type
	{
		player = 0,
		ninjaPommey = 1,
		sword = 2,
		spawner = 3
	};

	long long id;
	Type type;
	std::vector<Action> actionLog;
	double rotation;
};