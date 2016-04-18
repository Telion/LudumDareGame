#include "Client.h"

#include "../common/VectorMath.h"
#include "../common/Packet.h"
#include "../common/ProcessSocket.h"
#include "../common/Action.h"
#include "../common/Config.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <emscripten.h>
#include <unordered_set>

Client::Client()
{
	TRACE;

	socket.reset(new ProcessSocket());
	std::static_pointer_cast<ProcessSocket>(socket)->linkToServer(std::static_pointer_cast<ProcessSocket>(socket));

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	characterSprites = UniformSpriteSheet(renderer, "../../src/client/resources/characterSprites.png", 96, 96);
	terrainSprites = UniformSpriteSheet(renderer, "../../src/client/resources/terrainSprites.png", 64, 64);
	enemySprites = UniformSpriteSheet(renderer, "../../src/client/resources/enemySprites.png", 96, 96);

	lastTime = emscripten_get_now() * 1000;
	remainderTime = 0;
	lastTickTime = lastTime;

	positionLastTimestep = character.getPosition();
}

Client::~Client()
{
	TRACE;

	IMG_Quit();
	SDL_Quit();
}

void Client::gameLoop()
{
	TRACE;

	// For built-in server only
	server.serverLoop();

	unsigned time = emscripten_get_now() * 1000;
	int delta = time - lastTime;
	lastTime = time;

	if (delta < 0)
		delta = 0;

	if (delta > 300000)
		delta = 300000;

	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(nullptr);

	readPackets();
	world.updateLoadedChunks(character.getPosition(), socket);

	remainderTime += delta;
	while (remainderTime >= timestep)
	{
		positionLastTimestep = character.getPosition();
		tick(timestep);

		Action action;
		action.time = time - remainderTime;
		action.position = character.getPosition();
		actionLog.push_back(action);

		remainderTime -= timestep;
	}

	if (time - lastTickTime >= tickTime)
	{
		sendTick();
	}

	// We always render one timestep behind.
	render(time - timestep);
}

void Client::sendTick()
{
	TRACE;

	Packet packet;
	packet.type = Packet::Type::clientTick;
	packet.actionLog = actionLog;

	socket->send(packet);

	lastTickTime = lastTime;
	actionLog.clear();
}

void Client::readPackets()
{
	TRACE;

	Packet packet = socket->receive();
	while (packet.type != Packet::Type::none)
	{
		switch (packet.type)
		{
		case Packet::Type::connect:
			character.id = packet.id;
			break;
		case Packet::Type::chunk:
			handleChunk(packet);
			break;
		case Packet::Type::serverTick:
			handleServerTick(packet);
			break;
		default:
			printf("Client received unsupported packet type: %d\n", packet.type);
		}

		packet = socket->receive();
	}
}

void Client::handleChunk(const Packet& packet)
{
	TRACE;

	world.setChunk(packet.chunkX, packet.chunkY, Chunk(packet.chunkX, packet.chunkY, packet.tiles));
}

void Client::handleServerTick(const Packet& packet)
{
	TRACE;

	for (const auto& entity : packet.entities)
	{
		if (entity.id == character.id)
			continue;

		if (entities.find(entity.id) != entities.end())
		{
			CommonEntity& existingEntity = entities[entity.id];

			//// Erase old actions that are no longer relevant
			//std::size_t i = 0;
			//for (; i < existingEntity.actionLog.size(); ++i)
			//{
			//	if (existingEntity.actionLog[i].time >= lastTime - serverRenderingLag)
			//		break;
			//}
			//if (i + 1 >= existingEntity.actionLog.size())
			//	existingEntity.actionLog.clear();
			//else if (i >= 1)
			//	existingEntity.actionLog = std::vector<Action>(existingEntity.actionLog.begin() + i - 1, existingEntity.actionLog.end());

			// Append the new actions from the server
			for (const auto& action : entity.actionLog)
			{
				existingEntity.actionLog.push_back(action);
			}
		}
		else
		{
			entities[entity.id] = entity;
		}
	}

	//// Erase entities that were not in the latest update.
	//std::unordered_set<long long> existingIds;
	//for (const auto& entity : entities)
	//	existingIds.insert(entity.second.id);

	//std::unordered_set<long long> newIds;
	//for (const auto& entity : packet.entities)
	//	newIds.insert(entity.id);

	//existingIds.erase(newIds.begin(), newIds.end());

	//for (long long id : existingIds)
	//	entities.erase(id);
}

void Client::tick(int microseconds)
{
	TRACE;

	character.tick(microseconds, keys, world);
}

void Client::render(unsigned time)
{
	TRACE;

	double factor = (double)remainderTime / timestep;
	Vector2 base = lerp(positionLastTimestep, character.getPosition(), factor);
	base.x -= (screenWidth - 96) / 2;
	base.y -= (screenHeight - 96) / 2;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, nullptr);

	world.render(renderer, terrainSprites, base, screenWidth, screenHeight);

	character.render(renderer, characterSprites, entityTypeToSprite(CommonEntity::Type::player), base, factor);

	for (auto& e : entities)
		renderCommonEntity(renderer, e.second, time - interpolationDelay * 2, base);

	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
}

void Client::renderCommonEntity(SDL_Renderer* renderer, const CommonEntity& entity, unsigned time, Vector2 base)
{
	TRACE;

	if (entity.actionLog.size() == 0)
		return;

	unsigned startTime = entity.actionLog.front().time;
	if (time <= startTime)
	{
		characterSprites.renderSprite(entityTypeToSprite(entity.type), entity.actionLog.front().position, base, renderer);
		return;
	}

	// Find the first action long with a timestamp which is at least the current rendering time
	std::size_t i = 0;
	for (; i < entity.actionLog.size(); ++i)
	{
		if (entity.actionLog[i].time >= time)
			break;
	}

	if (i == entity.actionLog.size())
	{
		characterSprites.renderSprite(entityTypeToSprite(entity.type), entity.actionLog.back().position, base, renderer);
		return;
	}

	double fractionNew = ((double)time - entity.actionLog[i - 1].time) / (entity.actionLog[i].time - entity.actionLog[i - 1].time);
	Vector2 p = lerp(entity.actionLog[i - 1].position, entity.actionLog[i].position, fractionNew);

	characterSprites.renderSprite(entityTypeToSprite(entity.type), p, base, renderer);
}

int Client::entityTypeToSprite(CommonEntity::Type type) const
{
	TRACE;

	switch (type)
	{
	case CommonEntity::Type::player:
		return 1;
	case CommonEntity::Type::ninjaPommey:
		return 3;
	case CommonEntity::Type::sword:
		return 2;
	}
}