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

public:
	CommonEntity common;

	ServerEntity(CommonEntity::Type type, Vector2 position);

	virtual void tick(unsigned time, int delta, const World& world, std::vector<std::shared_ptr<ServerEntity>>& entities);
};