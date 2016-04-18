#pragma once

#include "../common/Socket.h"
#include "ServerEntity.h"

#include <memory>

class ServerClient
{
public:
	ServerClient();

	int id;
	std::shared_ptr<Socket> socket;
	std::shared_ptr<ServerEntity> entity;
};