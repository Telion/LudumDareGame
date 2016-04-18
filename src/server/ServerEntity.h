#pragma once

#include "../common/VectorMath.h"
#include "../common/World.h"
#include "../common/CommonEntity.h"

#include <memory>
#include <vector>

class World;

class ServerEntity
{
protected:
	Vector2 position;

	std::vector<std::shared_ptr<ServerEntity>> getEntitiesWithin(const std::vector<std::shared_ptr<ServerEntity>>& entities, double distance, CommonEntity::Type type) const;
	void handlePlayerMove(unsigned time);

public:
	CommonEntity common;

	// For shots
	Vector2 shotDirection;
	unsigned createTime;
	const int shotLifetime = 1500000;

	// For spawners
	unsigned lastSpawn;
	const int spawnRate = 2000000;

	ServerEntity(CommonEntity::Type type, Vector2 position);

	virtual void tick(unsigned time, int delta, const World& world, std::vector<std::shared_ptr<ServerEntity>>& entities, std::vector<std::shared_ptr<ServerEntity>>& newEntities, std::vector<const ServerEntity*>& deletedEntities);
};