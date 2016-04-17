#pragma once

#include "ClientCharacter.h"
#include "UniformSpriteSheet.h"
#include "../common/Chunk.h"
#include "../common/Socket.h"
#include "../common/Packet.h"
#include "../server/Server.h"
#include "../common/World.h"
#include "../common/VectorMath.h"

#include <SDL/SDL.h>
#include <utility>
#include <map>
#include <memory>

class Client
{
	Server server;

	std::shared_ptr<Socket> socket;

	SDL_Window* window;
	SDL_Renderer* renderer;
	const unsigned char* keys;

	unsigned lastTime;
	unsigned remainderTime;

	Position positionLastTimestep;

	UniformSpriteSheet characterSprites;
	UniformSpriteSheet terrainSprites;

	ClientCharacter character;
	World world;

	std::vector<ClientAction> actionLog;
	unsigned lastTickTime;

	void handleChunk(const Packet& packet);
	void sendTick();

	static const unsigned timestep = 25000;

public:
	Client();

	~Client();
	
	void gameLoop();

	void readPackets();
	void tick(int microseconds);
	void render();

	static const int screenWidth = 960;
	static const int screenHeight = 720;
};