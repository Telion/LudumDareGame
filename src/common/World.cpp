#include "World.h"

#include "../common/MyAssert.h"

bool World::hasChunk(int x, int y) const
{
	return chunks.find(std::make_pair(x, y)) != chunks.end();
}

Chunk& World::getChunk(int x, int y)
{
	MyAssert(hasChunk(x, y));

	return chunks[std::make_pair(x, y)];
}

const Chunk& World::getChunk(int x, int y) const
{
	MyAssert(hasChunk(x, y));

	return chunks.find(std::make_pair(x, y))->second;
}

void World::setChunk(int x, int y, const Chunk& chunk)
{
	chunks[std::make_pair(x, y)] = chunk;
}

Tile World::getTile(int x, int y) const
{
	auto chunkCoords = tileToChunkCoordinates(x, y, Chunk::chunkWidth, Chunk::chunkHeight);

	if (!hasChunk(chunkCoords.first, chunkCoords.second))
	{
		Tile tile;
		tile.id = -1;
		return tile;
	}

	const Chunk& chunk = getChunk(chunkCoords.first, chunkCoords.second);

	if (!chunk.isValid())
	{
		Tile tile;
		tile.id = -1;
		return tile;
	}

	x -= chunkCoords.first * Chunk::chunkWidth;
	y -= chunkCoords.second * Chunk::chunkHeight;

	return chunk.getTile(x, y);
}

void World::updateLoadedChunks(Position playerPosition, const std::shared_ptr<Socket>& socket)
{
	auto playerChunk = positionToChunkCoordinates(playerPosition, Chunk::chunkWidth, Chunk::chunkHeight);

	std::vector<std::pair<int, int>> chunksToRemove;

	for (auto p : chunks)
	{
		if (p.first.first < playerChunk.first - 2 || p.first.first > playerChunk.first + 2 || p.first.second < playerChunk.second - 2 || p.first.second > playerChunk.second + 2)
			chunksToRemove.push_back(p.first);
	}

	for (auto p : chunksToRemove)
	{
		printf("Erasing chunk!\n");
		chunks.erase(p);
	}

	for (int i = playerChunk.first - 1; i <= playerChunk.first + 1; ++i)
		for (int j = playerChunk.second - 1; j <= playerChunk.second + 1; ++j)
		{
			if (chunks.find(std::make_pair(i, j)) == chunks.end())
			{
				Chunk loadingChunk;
				loadingChunk.markLoading();
				chunks[std::make_pair(i, j)] = loadingChunk;

				Packet packet;
				packet.type = Packet::Type::chunk;
				packet.chunkX = i;
				packet.chunkY = j;
				socket->send(packet);
			}
		}
}

void World::render(SDL_Renderer* renderer, const UniformSpriteSheet& terrainSprites, Position base, int screenWidth, int screenHeight) const
{
	for (auto p : chunks)
	{
		p.second.render(renderer, terrainSprites, base, screenWidth, screenHeight);
	}
}