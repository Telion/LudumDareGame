#pragma once

#include "ClientCharacter.h"
#include "UniformSpriteSheet.h"
#include "../common/Chunk.h"
#include "../common/Socket.h"
#include "../common/Packet.h"
#include "../server/Server.h"

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
	unsigned char* keys;

	unsigned lastTime;

	UniformSpriteSheet characterSprites;
	UniformSpriteSheet terrainSprites;

	ClientCharacter character;
	std::map<std::pair<int, int>, Chunk> chunks;

	void handleChunk(const Packet& packet);

public:
	Client();

	~Client();
	
	void gameLoop();

	void readPackets();
	void manageChunks();
	void tick(int microseconds);
	void render();

	static const int screenWidth = 960;
	static const int screenHeight = 720;
};