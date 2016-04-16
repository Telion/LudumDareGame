#pragma once

#include "Packet.h"

#include <vector>

class Socket
{
public:
	virtual void send(const Packet& packet) = 0;
	virtual Packet receive() = 0;

protected:
	std::vector<unsigned char> packetToBytes(const Packet&)
	{
		return std::vector<unsigned char>();
	}

	Packet bytesToPacket(const std::vector<unsigned char>&)
	{
		return Packet();
	}
};