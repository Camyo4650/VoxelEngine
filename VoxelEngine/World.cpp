#include "World.h"
#include <array>
#include <string>
#include "VBO.h"
#include <future>

Game::World::World(Player* localPlayer) :
	localPlayer(localPlayer),
	texture("resources/texturepack-simple.png"),
	chunkVBO(),
	terrain(),
	lastPlayerPos(localPlayer->getChunkCoords())
{
	this->loadChunks();
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

void Game::World::generateChunkAsync(Game::World* world, Game::Chunk* c)
{
	c->generateTerrain(world->terrain.getHeightMap(c->chunkPos));
	if (!c->isEmpty())
	{
		c->generateCaves(world->terrain.getCaves(c->chunkPos));
	}
}

void Game::World::loadChunks()
{
	ChunkPos plrCoords = localPlayer->getChunkCoords();
	// within CHUNK_RENDER_DISTANCE
	for (int y = -CHUNK_RENDER_DISTANCE; y <= CHUNK_RENDER_DISTANCE; y++)
	{
		for (int x = -CHUNK_RENDER_DISTANCE; x <= CHUNK_RENDER_DISTANCE; x++)
		{
			if (x * x + y * y <= CHUNK_RENDER_DISTANCE * CHUNK_RENDER_DISTANCE)
			{
				for (int z = -1 - CHUNK_RENDER_DISTANCE / 2; z <= 1 + CHUNK_RENDER_DISTANCE / 2; z++)
				{
					ChunkPos pos = { plrCoords.X + x, plrCoords.Y + y, plrCoords.Z + z };
					int ID = Chunk::getChunkId(pos);
					if (renderedChunks.contains(ID)) continue;
					if (z < 0) continue;
					if (z >= ChunkPos::MaxZ) break;
					Chunk* c = new Chunk(texture, pos, &chunkVBO);
					newChunks[ID] = c;
					auto handle = std::async(std::launch::async, [this, c] { generateChunkAsync(this, c); qChunks.push(c); });
				}
			}
		}
	}

	for (auto& chunk : newChunks)
	{
		Chunk* c = chunk.second;

		int x  = Chunk::getChunkId({ c->chunkPos.X - 1, c->chunkPos.Y, c->chunkPos.Z });
		int x1 = Chunk::getChunkId({ c->chunkPos.X + 1, c->chunkPos.Y, c->chunkPos.Z });
		int y  = Chunk::getChunkId({ c->chunkPos.X, c->chunkPos.Y - 1, c->chunkPos.Z });
		int y1 = Chunk::getChunkId({ c->chunkPos.X, c->chunkPos.Y + 1, c->chunkPos.Z });
		int z  = Chunk::getChunkId({ c->chunkPos.X, c->chunkPos.Y, (int8_t)(c->chunkPos.Z - 1) });
		int z1 = Chunk::getChunkId({ c->chunkPos.X, c->chunkPos.Y, (int8_t)(c->chunkPos.Z + 1) });

		if (newChunks.contains(x))
			c->Cx = newChunks[x];
		if (newChunks.contains(x1))
			c->Cx1 = newChunks[x1];
		if (newChunks.contains(y))
			c->Cy = newChunks[y];
		if (newChunks.contains(y1))
			c->Cy1 = newChunks[y1];
		if (newChunks.contains(z))
			c->Cz = newChunks[z];
		if (newChunks.contains(z1))
			c->Cz1 = newChunks[z1];
		// now check already rendered chunks
		
		if (renderedChunks.contains(x))
		{
			c->Cx = renderedChunks[x];
			renderedChunks[x]->Cx1  = c;
			qChunks.push(renderedChunks[x]);
		}
		if (renderedChunks.contains(x1))
		{
			c->Cx1 = renderedChunks[x1];
			renderedChunks[x1]->Cx  = c;
			qChunks.push(renderedChunks[x1]);
		}
		if (renderedChunks.contains(y))
		{
			c->Cy = renderedChunks[y];
			renderedChunks[y]->Cy1  = c;
			qChunks.push(renderedChunks[y]);
		}
		if (renderedChunks.contains(y1))
		{
			c->Cy1 = renderedChunks[y1];
			renderedChunks[y1]->Cy  = c;
			qChunks.push(renderedChunks[y1]);
		}
		if (renderedChunks.contains(z))
		{
			c->Cz = renderedChunks[z];
			renderedChunks[z]->Cz1  = c;
			qChunks.push(renderedChunks[z]);
		}
		if (renderedChunks.contains(z1))
		{
			c->Cz1 = renderedChunks[z1];
			renderedChunks[z1]->Cz  = c;
			qChunks.push(renderedChunks[z1]);
		}
	}
	newChunks.clear();
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
	int curSize = qChunks.size();
	while (!qChunks.empty() && (int)qChunks.size() > curSize / 2) { // aim to shrink it by 1/2 each cycle
		Chunk* c = qChunks.front();
		qChunks.pop();
		int t = c->ID();
		renderedChunks[t] = c;
		c->generateVertices();
		//auto handle = std::async(std::launch::async, [c] { c->generateVertices(); });
	}
}

