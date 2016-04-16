#pragma once

#include "Socket.h"
#include "Packet.h"

#include <queue>

class ProcessSocket : public Socket
{
	std::queue<Packet> receiveBuffer;
	std::weak_ptr<ProcessSocket> pairedSocket;

public:
	ProcessSocket();

	void linkToServer(const std::shared_ptr<ProcessSocket>& sharedThis);
	void linkToClient(const std::shared_ptr<ProcessSocket>& client);

	void send(const Packet& packet) override;
	Packet receive() override;

	void pushPacket(const Packet& packet);
};