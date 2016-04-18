#include "Server.h"

#include "ClientManager.h"
#include "../common/Socket.h"
#include "../common/MyAssert.h"
#include "../common/Config.h"

#include <emscripten.h>
#include "../libs/tinyxml2.h"
#include <fstream>
#include <sstream>

Server::Server()
{
	TRACE;

	entities.push_back(std::shared_ptr<ServerEntity>(new ServerEntity(CommonEntity::Type::ninjaPommey, Vector2())));

	lastTime = emscripten_get_now() * 1000;
	remainderTime = 0;
	lastTickTime = lastTime;
}

void Server::serverLoop()
{
	TRACE;

	unsigned time = emscripten_get_now() * 1000;
	int delta = time - lastTime;
	lastTime = time;

	if (delta < 0)
		delta = 0;

	if (delta > 300000)
		delta = 300000;

	remainderTime += delta;
	while (remainderTime >= timestep)
	{
		tick(time - remainderTime, timestep);
		
		remainderTime -= timestep;
	}

	if (time - lastTickTime >= tickTime)
	{
		sendTicks();
	}
}

void Server::sendTicks()
{
	TRACE;

	lastTickTime = lastTime;

	Packet packet;
	packet.type = Packet::Type::serverTick;

	for (auto& e : entities)
	{
		packet.entities.push_back(e->common);

		e->common.actionLog.clear();
	}

	for (ServerClient& client : getClientManager().getClients())
	{
		client.socket->send(packet);
	}
}

void Server::tick(unsigned time, int delta)
{
	TRACE;

	handlePackets();

	for (std::size_t i = 0; i < entities.size(); ++i)
	{
		entities[i]->tick(time - interpolationDelay, delta, world, entities);
	}
}

void Server::handlePackets()
{
	TRACE;

	auto& clients = getClientManager().getClients();

	for (ServerClient& client : clients)
	{
		if (!client.entity)
		{
			// Create an entity for this new player
			client.entity.reset(new ServerEntity(CommonEntity::Type::player, Vector2()));
			entities.push_back(client.entity);

			Packet packet;
			packet.type = Packet::Type::connect;
			packet.id = client.entity->common.id;
			client.socket->send(packet);
		}

		Packet packet = client.socket->receive();
		while (packet.type != Packet::Type::none)
		{
			switch (packet.type)
			{
			case Packet::Type::chunk:
				handleChunk(client, packet);
				break;
			case Packet::Type::clientTick:
				handleClientTick(client, packet);
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
	TRACE;

	if (!world.hasChunk(packet.chunkX, packet.chunkY))
	{
		std::stringstream filename;
		filename << "../../src/server/maps/" << packet.chunkX << ',' << packet.chunkY << ".tmx";

		std::ifstream testFile(filename.str());
		if (testFile.good())
		{
			testFile.close();

			loadChunk(packet.chunkX, packet.chunkY, filename.str());
		}
		else
		{
			testFile.close();

			world.setChunk(packet.chunkX, packet.chunkY, Chunk());
		}
	}

	Chunk& chunk = world.getChunk(packet.chunkX, packet.chunkY);
	
	Packet response;
	response.type = Packet::Type::chunk;
	response.chunkX = packet.chunkX;
	response.chunkY = packet.chunkY;
	response.tiles = chunk.getTiles();

	client.socket->send(response);
}

void Server::handleClientTick(const ServerClient& client, const Packet& packet)
{
	TRACE;

	auto& actionLog = client.entity->common.actionLog;

	for (auto& action : packet.actionLog)
		actionLog.push_back(action);
}

std::vector<Tile> parseCsv(const std::string& str, int firstgid)
{
	TRACE;

	std::stringstream ss(str);
	std::vector<Tile> result;

	while (ss)
	{
		char nextChar = ss.peek();
		if (nextChar >= '0' && nextChar <= '9')
		{
			Tile tile;
			ss >> tile.id;
			tile.id -= firstgid;
			result.push_back(tile);
		}
		else
		{
			ss.get();
		}
	}

	return result;
}

void Server::loadChunk(int x, int y, const std::string& filename)
{
	TRACE;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(filename.c_str());
	MyAssert(error == tinyxml2::XMLError::XML_SUCCESS);

	tinyxml2::XMLElement* map = doc.FirstChildElement("map");
	tinyxml2::XMLElement* tileset = map->FirstChildElement("tileset");
	int firstgid;
	error = tileset->QueryIntAttribute("firstgid", &firstgid);
	MyAssert(error == tinyxml2::XMLError::XML_SUCCESS);

	tinyxml2::XMLElement* layer = map->FirstChildElement("layer");
	tinyxml2::XMLElement* data = layer->FirstChildElement("data");
	std::string text = data->GetText();

	std::vector<Tile> tiles = parseCsv(text, firstgid);

	MyAssert(tiles.size() == Chunk::chunkWidth * Chunk::chunkHeight);

	world.setChunk(x, y, Chunk(x, y, tiles));
}