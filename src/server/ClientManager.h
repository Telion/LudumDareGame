#pragma once

#include "../common/Socket.h"
#include "../common/ProcessSocket.h"
#include "ServerClient.h"

#include <memory>

class ClientManager
{
	std::vector<ServerClient> clients;

public:
	std::vector<ServerClient>& getClients();

	std::weak_ptr<ProcessSocket> connectFromProcessSocket(std::shared_ptr<ProcessSocket> clientSocket);
};

ClientManager& getClientManager();
