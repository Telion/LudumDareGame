#pragma once

#include "ConnectionManager.h"

#include <string>

//bool serverSent = false;
//bool serverReceived = false;

class Server
{
public:
	//static void testTick()
	//{
	//	std::shared_ptr<Socket> socket = connectionManager.getSockets().front();

	//	if (!serverReceived)
	//	{
	//		Packet packet = socket->receive();
	//		if (packet.type == Packet::Type::test)
	//		{
	//			printf("Received %s\n", packet.message.c_str());
	//			serverReceived = true;

	//			packet = socket->receive();
	//			printf("Received %s\n", packet.message.c_str());
	//		}
	//	}

	//	if (serverReceived && !serverSent)
	//	{
	//		Packet packet;
	//		packet.type = Packet::Type::test;
	//		packet.message = "Hello client!";
	//		socket->send(packet);
	//		serverSent = true;
	//	}
	//}
};