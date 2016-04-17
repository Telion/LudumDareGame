#include "Client.h"

#include "../common/VectorMath.h"
#include "../common/Packet.h"
#include "../common/ProcessSocket.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <emscripten.h>

Client::Client()
{
	socket.reset(new ProcessSocket());
	std::static_pointer_cast<ProcessSocket>(socket)->linkToServer(std::static_pointer_cast<ProcessSocket>(socket));

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	characterSprites = UniformSpriteSheet(renderer, "../../src/client/resources/samurai.png", 96, 96);
	terrainSprites = UniformSpriteSheet(renderer, "../../src/client/resources/terrainSprites.png", 64, 64);

	lastTime = emscripten_get_now() * 1000;
}

Client::~Client()
{
	IMG_Quit();
	SDL_Quit();
}

void Client::gameLoop()
{
	unsigned time = emscripten_get_now() * 1000;
	int delta = time - lastTime;
	lastTime = time;

	if (delta < 0)
		delta = 0;

	if (delta > 300000)
		delta = 300000;

	server.tick(delta);

	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(nullptr);

	readPackets();
	world.updateLoadedChunks(character.getPosition(), socket);
	tick(delta);
	render();
}

void Client::readPackets()
{
	Packet packet = socket->receive();
	while (packet.type != Packet::Type::none)
	{
		switch (packet.type)
		{
		case Packet::Type::chunk:
			handleChunk(packet);
			break;
		default:
			printf("Client received unsupported packet type: %d", packet.type);
		}

		packet = socket->receive();
	}
}

void Client::handleChunk(const Packet& packet)
{
	world.setChunk(packet.chunkX, packet.chunkY, Chunk(packet.chunkX, packet.chunkY, packet.tiles));
}

void Client::tick(int microseconds)
{
	character.tick(microseconds, keys, world);
}

void Client::render()
{
	Position base;
	base.x = character.getPosition().x - (screenWidth - 96) / 2;
	base.y = character.getPosition().y - (screenHeight - 96) / 2;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, nullptr);

	world.render(renderer, terrainSprites, base, screenWidth, screenHeight);

	character.render(renderer, characterSprites, base);

	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
}
