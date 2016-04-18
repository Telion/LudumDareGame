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
	bool mouseClick;
	int mouseClickX;
	int mouseClickY;
	bool mouseDown;
	int mouseX;
	int mouseY;

	unsigned lastTime;
	unsigned remainderTime;

	Vector2 positionLastTimestep;

	UniformSpriteSheet characterSprites;
	UniformSpriteSheet terrainSprites;
	UniformSpriteSheet enemySprites;

	ClientCharacter character;
	World world;
	std::map<long long, CommonEntity> entities;

	unsigned lastTickTime;

	void handleChunk(const Packet& packet);
	void handleServerTick(const Packet& packet);
	void sendTick();
	void renderCommonEntity(SDL_Renderer* renderer, const CommonEntity& entity, unsigned time, Vector2 base);
	int entityTypeToSprite(CommonEntity::Type type) const;

	static const unsigned timestep = 25000;
	static const unsigned tickTime = 150000;
	static const unsigned serverRenderingLag = 300000;

public:
	Client();

	~Client();
	
	void gameLoop();

	void readPackets();
	void tick(unsigned time, int delta);
	void render(unsigned time);
};