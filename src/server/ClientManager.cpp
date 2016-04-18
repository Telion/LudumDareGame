#include "ClientManager.h"

#include "ServerEntity.h"

ClientManager& getClientManager()
{
	static ClientManager clientManager;
	return clientManager;
}

std::vector<ServerClient>& ClientManager::getClients()
{
	return clients;
}

std::weak_ptr<ProcessSocket> ClientManager::connectFromProcessSocket(std::shared_ptr<ProcessSocket> clientSocket)
{
	std::shared_ptr<ProcessSocket> serverSocket(new ProcessSocket());
	serverSocket->linkToClient(clientSocket);

	ServerClient newClient;
	newClient.socket = serverSocket;
	clients.push_back(newClient);
	getClients();

	return std::weak_ptr<ProcessSocket>(serverSocket);
}