#pragma once

#include "ClientManager.h"
#include "../common/Chunk.h"
#include "ServerClient.h"
#include "../common/Packet.h"

#include <string>
#include <utility>
#include <map>

class Server
{
	void handleChunk(const ServerClient& client, const Packet& packet);
	void handleClientTick(const ServerClient& client, const Packet& packet);

	std::map<std::pair<int, int>, Chunk> chunkCache;

public:
	Server();

	void tick(int microseconds);
};