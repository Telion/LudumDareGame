#include "../common/ProcessSocket.h"
#include "../server/Server.h"
#include "Client.h"

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <memory>

#include <emscripten.h>

//std::shared_ptr<ProcessSocket> clientSocket;
//bool sent = false;
//bool received = false;

//void testTick()
//{
//	if (!sent)
//	{
//		Packet packet;
//		packet.type = Packet::Type::test;
//		packet.message = "Hello server!";
//		clientSocket->send(packet);
//		packet.message = "Hello server again!";
//		clientSocket->send(packet);
//		sent = true;
//	}
//	else if (!received)
//	{
//		Packet packet = clientSocket->receive();
//		if (packet.type == Packet::Type::test)
//		{
//			printf("Received %s\n", packet.message.c_str());
//			received = true;
//		}
//	}
//}

Client globalClient;

void gameLoop()
{
	globalClient.gameLoop();
}

extern "C" int main(int argc, char** argv)
{
	//clientSocket.reset(new ProcessSocket());
	//clientSocket->linkToServer(clientSocket);

	//while (!sent || !received)
	//{
	//	testTick();
	//	Server::testTick();
	//}



	//SDL_Init(SDL_INIT_VIDEO);
	//IMG_Init(IMG_INIT_PNG);

	//SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 768, 576, SDL_WINDOW_SHOWN);
	//SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

	//SDL_Surface* image = IMG_Load("../../src/client/resources/test.png");
	//SDL_BlitSurface(image, nullptr, windowSurface, nullptr);

	//SDL_UpdateWindowSurface(window);

	//IMG_Quit();
	//SDL_Quit();

	emscripten_set_main_loop(gameLoop, 0, false);

	return 0;
}
