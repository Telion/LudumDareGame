#include "ServerClient.h"

ServerClient::ServerClient()
{
	static int nextId = 0;

	id = nextId++;
}