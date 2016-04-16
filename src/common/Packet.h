#pragma once

#include <string>

struct Packet
{
	enum class Type
	{
		none,
		test,
		//chunk,
		//clientTick,
		//serverTick
	};

	Type type;

	std::string message;

	static Packet empty()
	{
		Packet packet;
		packet.type = Type::none;

		return packet;
	}
};
