#include "ServerEntity.h"

#include "../common/CollisionResolution.h"
#include "Server.h"

#include <emscripten.h>
#include <cstdlib>

ServerEntity::ServerEntity(CommonEntity::Type type, Vector2 position)
{
	static long long nextId = 0;
	common.id = nextId++;
	common.type = type;
	common.rotation = 0;
	this->position = position;
	this->lastSpawn = 0;
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

void ServerEntity::tick(unsigned time, int delta, const World& world, std::vector<std::shared_ptr<ServerEntity>>& entities, std::vector<std::shared_ptr<ServerEntity>>& newEntities, std::vector<const ServerEntity*>& deletedEntities)
{
	if (common.type == CommonEntity::Type::player)
	{
		if (common.actionLog.size() == 0)
			return;

		handlePlayerMove(time);

		// Find the first action that was after the previous timestep
		std::size_t i;
		for (i = 0; i < common.actionLog.size(); ++i)
		{
			if (common.actionLog[i].time > time - delta)
				break;
		}
		for (; i < common.actionLog.size() && common.actionLog[i].time <= time; ++i)
		{
			// Check if we need to fire a shot.
			if (common.actionLog[i].shooting)
			{
				std::shared_ptr<ServerEntity> projectile(new ServerEntity(CommonEntity::Type::sword, position));
				projectile->shotDirection = common.actionLog[i].shotDirection;
				projectile->common.rotation = atan2(projectile->shotDirection.y, projectile->shotDirection.x);
				projectile->createTime = time;
				newEntities.push_back(projectile);
			}
		}

		return;
	}
	else if (common.type == CommonEntity::Type::sword)
	{
		if (time > createTime + shotLifetime)
			deletedEntities.push_back(this);

		const double speed = 275;

		Vector2 movement = normalize(shotDirection) * speed * delta / 1000000;

		Rect current(position.x, position.y, 96, 96);
		Rect dest(position.x + movement.x, position.y + movement.y, 96, 96);

		if (collides(current, dest, world))
		{
			deletedEntities.push_back(this);
		}

		auto touchingEnemies = getEntitiesWithin(entities, 48, CommonEntity::Type::ninjaPommey);
		if (!touchingEnemies.empty())
		{
			deletedEntities.push_back(&*touchingEnemies.front());
			deletedEntities.push_back(this);
		}

		auto touchingSpawners = getEntitiesWithin(entities, 48, CommonEntity::Type::spawner);
		if (!touchingSpawners.empty())
		{
			deletedEntities.push_back(&*touchingSpawners.front());
			deletedEntities.push_back(this);
		}

		position = Vector2(dest.x, dest.y);
	}
	else if (common.type == CommonEntity::Type::spawner)
	{
		if (time > lastSpawn + spawnRate)
		{
			lastSpawn = time;

			auto nearbyEnemies = getEntitiesWithin(entities, 96, CommonEntity::Type::ninjaPommey);
			if (nearbyEnemies.size() < 3)
			{
				std::shared_ptr<ServerEntity> enemy(new ServerEntity(CommonEntity::Type::ninjaPommey, Vector2(position.x + (double)rand() / RAND_MAX * 128 - 64, position.y + (double)rand() / RAND_MAX * 128 - 64)));
				newEntities.push_back(enemy);
			}
		}
	}
	else
	{
		const double speed = 175;

		auto touchingPlayers = getEntitiesWithin(entities, 72, CommonEntity::Type::player);
		if (!touchingPlayers.empty())
		{
			EM_ASM(alert('Congratulations! You have shapeshifted into a dead body.'));
			emscripten_cancel_main_loop();
		}

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
	}

	Action action;
	action.time = time;
	action.position = position;
	common.actionLog.push_back(action);
}

void ServerEntity::handlePlayerMove(unsigned time)
{
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