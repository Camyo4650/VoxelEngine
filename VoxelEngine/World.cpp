#include "World.h"
#include <array>
#include <string>
#include "VBO.h"
#include <future>
#include "Constants.h"
#include <omp.h>

Game::World::World(Player* localPlayer) :
	localPlayer(localPlayer),
	texture("resources/texturepack-simple.png"),
	chunkVBO(),
	terrain(),
	lastPlayerPos(localPlayer->getChunkCoords()),
	renderedChunks()
{
}


void Game::World::update(double delta)
{
	if (localPlayer->getChunkCoords() != this->lastPlayerPos) 
	{
		this->lastPlayerPos = localPlayer->getChunkCoords();
		this->loadChunks();
	}
}

void Game::World::render()
{
	this->renderChunks();
	for (auto& chunk : renderedChunks)
	{
		chunk.second->draw(localPlayer->camera.view, localPlayer->camera.proj);
	}
}

void Game::World::load()
{
	this->loadChunks();
}

uint16_t Game::World::getBlockId(const ChunkPos& chunkPos, int x, int y, int z) {
	ChunkPos adjustedChunkPos = { chunkPos.x, chunkPos.y, chunkPos.z };

	// Handle boundary crossing
	if (x < 0) { adjustedChunkPos.x--; x += C_sizeX; }
	if (x >= C_sizeX) { adjustedChunkPos.x++; x -= C_sizeX; }
	if (y < 0) { adjustedChunkPos.y--; y += C_sizeY; }
	if (y >= C_sizeY) { adjustedChunkPos.y++; y -= C_sizeY; }
	if (z < 0) { adjustedChunkPos.z--; z += C_sizeZ; }
	if (z >= C_sizeZ) { adjustedChunkPos.z++; z -= C_sizeZ; }

	// Check if the adjusted chunk exists
	auto it = this->newChunks.find(adjustedChunkPos);
	if (it == this->newChunks.end())
	{
		Chunk* c = this->loadChunkAt(adjustedChunkPos);
		return c->getBlockId(x, y, z);
	}

	// Delegate to the chunk-level `getBlockId`
	return it->second->getBlockId(x, y, z);
}

Game::Chunk* Game::World::loadChunkAt(const ChunkPos& pos)
{
	auto it = newChunks.find(pos);
	Chunk* c = NULL;
	if (it != newChunks.end()) c = newChunks[pos];
	else
	{
		c = new Chunk(this, pos, texture, &chunkVBO);
		c->generateTerrain(this->terrain.getHeightMap(pos));
		if (!c->isEmpty())
		{
			c->generateCaves(this->terrain.getCaves(pos));
		}
#pragma omp critical
		newChunks.emplace(pos, c);
	}
	return c;
}

void Game::World::loadChunks()
{
	ChunkPos plrCoords = localPlayer->getChunkCoords();
	// within CHUNK_RENDER_DISTANCE
#pragma omp parallel
	{
#pragma omp single
		{
			for (int y = -CHUNK_RENDER_DISTANCE; y <= CHUNK_RENDER_DISTANCE; y++)
			{
				for (int x = -CHUNK_RENDER_DISTANCE; x <= CHUNK_RENDER_DISTANCE; x++)
				{
					if (x * x + y * y <= 2 * CHUNK_RENDER_DISTANCE * CHUNK_RENDER_DISTANCE)
					{
						for (int z = -CHUNK_RENDER_DISTANCE / 2; z <= CHUNK_RENDER_DISTANCE / 2; z++)
						{
							ChunkPos pos = { plrCoords.x + x, plrCoords.y + y, plrCoords.z + z };
							if (pos.z < 0) continue;
							if (pos.z >= ChunkPos::MaxZ) break;
							Chunk* chunk = nullptr;
							if (!newChunks.contains(pos))
							{
								chunk = this->loadChunkAt(pos);
							}
							else
							{
								chunk = newChunks[pos];
							}
							#pragma omp critical
							qChunksRender.push(chunk);
						}
					}
				}
			}
		}
	}
}

void Game::World::renderChunks()
{
	// ideas:
	// start with knowing player position.
	// see all chunks that need to be created within the sphere of the player
		// make sure these chunks are within the confines of the world (int32 x, y. uint8 z)
	// we need to have a queue system to load chunks.

	// old code
	ChunkPos plrCoords = localPlayer->getChunkCoords();
	int j = 0;
	/*
	for (int x = -5; x < 5; x++)
	{
		for (int y = -5; y < 5; y++)
		{
			for (int z = 3; z < 6; z++)
			{
				ChunkPos pos = { plrCoords.X + x, plrCoords.Y + y, plrCoords.Z + z };
				int t = getChunkId(pos);
				renderedChunks[t] = new Chunk(texture, pos, &chunkVBO);
				renderedChunks[t]->generateTerrain(terrain.getHeightMap(pos));
				if (!renderedChunks[t]->isEmpty())
				{
					renderedChunks[t]->generateCaves(terrain.getCaves(pos));
				}
			}
		}
	}
	*/

	// NEW code
	int curSize = qChunksRender.size();
	int i = 0;
	while (!qChunksRender.empty() && i < curSize / 2) { // aim to shrink it by 1/2 each cycle
		Chunk* c = qChunksRender.front();
		qChunksRender.pop();
		renderedChunks.emplace(c->chunkPos, c);
		c->generateMesh();
		//auto handle = std::async(std::launch::async, [c] { c->generateVertices(); });
		i++;
	}
}

