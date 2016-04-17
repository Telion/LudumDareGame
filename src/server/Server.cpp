#include "Server.h"

#include "ClientManager.h"
#include "../common/Socket.h"

#include <fstream>
#include <sstream>

Server::Server()
{

}

void Server::tick(int /*microseconds*/)
{
	auto clients = getClientManager().getClients();

	for (ServerClient client : clients)
	{
		Packet packet = client.socket->receive();
		while (packet.type != Packet::Type::none)
		{
			switch (packet.type)
			{
			case Packet::Type::chunk:
				handleChunk(client, packet);
				break;
			default:
				printf("Server received unsupported packet type: %d from client %d\n", packet.type, client.id);
			}

			packet = client.socket->receive();
		}
	}
}

void Server::handleChunk(const ServerClient& client, const Packet& packet)
{
	auto p = std::make_pair(packet.chunkX, packet.chunkY);
	if (chunkCache.find(p) == chunkCache.end())
	{
		std::stringstream filename;
		filename << "../../src/server/maps/" << packet.chunkX << ',' << packet.chunkY << ".tmx";

		std::ifstream testFile(filename.str());
		if (testFile.good())
		{
			testFile.close();

			chunkCache[p] = Chunk(packet.chunkX, packet.chunkY, filename.str());
		}
		else
		{
			testFile.close();

			chunkCache[p] = Chunk();
		}
	}

	Chunk chunk = chunkCache[p];
	
	Packet response;
	response.type = Packet::Type::chunk;
	response.chunkX = packet.chunkX;
	response.chunkY = packet.chunkY;
	response.tiles = chunk.getTiles();

	client.socket->send(response);
}