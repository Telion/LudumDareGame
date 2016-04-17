#pragma once

#include "Tile.h"
#include "ClientAction.h"

#include <string>
#include <vector>

struct Packet
{
	enum class Type
	{
		none,
		//test,
		chunk,
		clientTick,
		//serverTick
	};

	Type type;

	// For test
	//std::string message;

	// For chunk
	int chunkX, chunkY;
	std::vector<Tile> tiles;

	// For clientTick
	std::vector<ClientAction> actionLog;

	static Packet empty()
	{
		Packet packet;
		packet.type = Type::none;

		return packet;
	}
};
