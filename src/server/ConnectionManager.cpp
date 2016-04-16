#include "ConnectionManager.h"

ConnectionManager connectionManager;

const std::vector<std::shared_ptr<Socket>>& ConnectionManager::getSockets() const
{
	return sockets;
}

std::weak_ptr<ProcessSocket> ConnectionManager::connectFromProcessSocket(std::shared_ptr<ProcessSocket> clientSocket)
{
	std::shared_ptr<ProcessSocket> serverSocket(new ProcessSocket());
	serverSocket->linkToClient(clientSocket);

	sockets.push_back(serverSocket);

	return std::weak_ptr<ProcessSocket>(serverSocket);
}