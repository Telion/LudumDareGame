#pragma once

#include "ClientManager.h"
#include "../common/World.h"
#include "ServerClient.h"
#include "../common/Packet.h"
#include "ServerEntity.h"

#include <string>
#include <utility>
#include <map>

class Server
{
	void tick(unsigned time, int delta);
	void handlePackets();
	void handleChunk(const ServerClient& client, const Packet& packet);
	void handleClientTick(const ServerClient& client, const Packet& packet);
	void loadChunk(int x, int y, const std::string& filename);
	void sendTicks();

	unsigned lastTime;
	unsigned remainderTime;
	unsigned lastTickTime;

	World world;

	std::vector<std::shared_ptr<ServerEntity>> entities;

	static const unsigned timestep = 25000;
	static const unsigned tickTime = 150000;

public:
	Server();

	void serverLoop();
};