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

	for (const auto& entity : newEntities)
	{
		entities.push_back(entity);
	}
	newEntities.clear();

	for (const auto& entity : deletedEntities)
	{
		for (std::size_t i = 0; i < entities.size(); ++i)
		{
			if (entities[i]->common.id == entity->common.id)
			{
				entities[i] = entities.back();
				entities.pop_back();
				break;
			}
		}
	}
	deletedEntities.clear();
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

		// Erase old actions that are no longer relevant
		std::size_t i = 0;
		for (; i < e->common.actionLog.size(); ++i)
		{
			if (e->common.actionLog[i].time >= lastTime - interpolationDelay * 2)
				break;
		}
		if (i + 1 >= e->common.actionLog.size())
			e->common.actionLog.clear();
		else if (i >= 1)
			e->common.actionLog = std::vector<Action>(e->common.actionLog.begin() + i - 1, e->common.actionLog.end());
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
		entities[i]->tick(time - interpolationDelay, delta, world, entities, newEntities, deletedEntities);
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
			client.entity.reset(new ServerEntity(CommonEntity::Type::player, Vector2(64 * 32, 64 * 45)));
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
	{
		for (std::size_t i = 0; i < actionLog.size(); ++i)
			if (actionLog[i].id == action.id)
				goto next;

		actionLog.push_back(action);

	next:;
	}
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
