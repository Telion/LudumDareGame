#include "ProcessSocket.h"

#include "../server/ConnectionManager.h"

ProcessSocket::ProcessSocket()
{
	
}

void ProcessSocket::linkToServer(const std::shared_ptr<ProcessSocket>& sharedThis)
{
	pairedSocket = connectionManager.connectFromProcessSocket(sharedThis);
}

void ProcessSocket::linkToClient(const std::shared_ptr<ProcessSocket>& client)
{
	pairedSocket = client;
}

void ProcessSocket::send(const Packet& packet)
{
	std::shared_ptr<ProcessSocket> pairedSocket = this->pairedSocket.lock();

	if (pairedSocket)
		pairedSocket->pushPacket(packet);
}

Packet ProcessSocket::receive()
{
	if (receiveBuffer.empty())
		return Packet::empty();

	Packet packet = receiveBuffer.front();
	receiveBuffer.pop();
	return packet;
}

void ProcessSocket::pushPacket(const Packet& packet)
{
	receiveBuffer.push(packet);
}