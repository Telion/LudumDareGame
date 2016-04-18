#pragma once

#include "Tile.h"
#include "Action.h"
#include "CommonEntity.h"

#include <string>
#include <vector>

struct Packet
{
	enum class Type
	{
		none,
		//test,
		connect,
		chunk,
		clientTick,
		serverTick
	};

	Type type;

	// For test
	//std::string message;

	// For connect
	long long id;

	// For chunk
	int chunkX, chunkY;
	std::vector<Tile> tiles;

	// For clientTick
	std::vector<Action> actionLog;

	// For serverTick
	std::vector<CommonEntity> entities;

	static Packet empty()
	{
		Packet packet;
		packet.type = Type::none;

		return packet;
	}
};
