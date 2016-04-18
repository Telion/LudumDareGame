#include "../common/ProcessSocket.h"
#include "../server/Server.h"
#include "Client.h"

#include <stdio.h>
#include <memory>

#include <emscripten.h>

void gameLoop()
{
	static Client client;
	client.gameLoop();
}

extern "C" int main(int, char**)
{
	emscripten_set_main_loop(gameLoop, 0, false);

	return 0;
}
