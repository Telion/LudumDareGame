#pragma once

#include "../common/Socket.h"
#include "../common/ProcessSocket.h"

#include <memory>

class ConnectionManager
{
	std::vector<std::shared_ptr<Socket>> sockets;

public:
	void addNewConnections();

	const std::vector<std::shared_ptr<Socket>>& getSockets() const;

	std::weak_ptr<ProcessSocket> connectFromProcessSocket(std::shared_ptr<ProcessSocket> clientSocket);
};

extern ConnectionManager connectionManager;