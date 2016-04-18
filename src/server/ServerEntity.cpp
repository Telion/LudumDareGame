#include "ServerEntity.h"

#include "../common/CollisionResolution.h"

ServerEntity::ServerEntity(CommonEntity::Type type, Vector2 position)
{
	static long long nextId = 0;
	common.id = nextId++;
	common.type = type;
	position = position;
}

std::vector<std::shared_ptr<ServerEntity>> ServerEntity::getEntitiesWithin(const std::vector<std::shared_ptr<ServerEntity>>& entities, double distance, CommonEntity::Type type) const
{
	std::vector<std::shared_ptr<ServerEntity>> result;

	for (std::size_t i = 0; i < entities.size(); ++i)
	{
		auto& entity = entities[i];
		if (entity->common.id != common.id && entity->common.type == type && entity->position.x >= position.x - distance && entity->position.x <= position.x + distance && entity->position.y >= position.y - distance && entity->position.y <= position.y + distance)
			result.push_back(entity);
	}

	return result;
}

void ServerEntity::tick(unsigned time, int delta, const World& world, std::vector<std::shared_ptr<ServerEntity>>& entities)
{
	if (common.type == CommonEntity::Type::player)
	{
		if (common.actionLog.size() == 0)
			return;

		unsigned startTime = common.actionLog.front().time;
		if (time <= startTime)
		{
			position = common.actionLog.front().position;
			return;
		}

		// Find the first action long with a timestamp which is at least the current rendering time
		std::size_t i = 0;
		for (; i < common.actionLog.size(); ++i)
		{
			if (common.actionLog[i].time >= time)
				break;
		}

		if (i == common.actionLog.size())
		{
			position = common.actionLog.back().position;
			return;
		}

		double fractionNew = ((double)time - common.actionLog[i - 1].time) / (common.actionLog[i].time - common.actionLog[i - 1].time);
		position = lerp(common.actionLog[i - 1].position, common.actionLog[i].position, fractionNew);
	}
	else
	{
		const double speed = 175;

		auto entitiesNearby = getEntitiesWithin(entities, 7.0 * 64, CommonEntity::Type::player);

		if (!entitiesNearby.empty())
		{
			ServerEntity& target = *entitiesNearby.front();

			Vector2 direction = target.position - position;

			if (magnitude(direction) > 4)
			{
				Vector2 movement = normalize(direction) * speed * delta / 1000000;

				Rect current(position.x, position.y, 96, 96);
				Rect dest(position.x + movement.x, position.y + movement.y, 96, 96);

				position = resolveCollisions(current, dest, world);
			}
		}

		Action action;
		action.time = time;
		action.position = position;
		common.actionLog.push_back(action);
	}
}