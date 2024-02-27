#include "World.h"
#include <array>
#include <string>
#include "VBO.h"

Game::World::World(Player* localPlayer) :
	localPlayer(localPlayer),
	texture("resources/texturepack-simple.png"),
	chunkVBO(),
	terrain()
{
	this->loadChunks();

	for (auto& chunk : renderedChunks)
	{
		Chunk* c = chunk.second;

		int x = Chunk::getChunkId({ c->chunkPos.X - 1, c->chunkPos.Y, c->chunkPos.Z });
		int x1 = Chunk::getChunkId({ c->chunkPos.X + 1, c->chunkPos.Y, c->chunkPos.Z });
		int y = Chunk::getChunkId({ c->chunkPos.X, c->chunkPos.Y - 1, c->chunkPos.Z });
		int y1 = Chunk::getChunkId({ c->chunkPos.X, c->chunkPos.Y + 1, c->chunkPos.Z });
		int z = Chunk::getChunkId({ c->chunkPos.X, c->chunkPos.Y, (int8_t)(c->chunkPos.Z - 1) });
		int z1 = Chunk::getChunkId({ c->chunkPos.X, c->chunkPos.Y, (int8_t)(c->chunkPos.Z + 1) });

		if (renderedChunks.contains(x))
			c->Cx = renderedChunks[x];
		if (renderedChunks.contains(x1))
			c->Cx1 = renderedChunks[x1];
		if (renderedChunks.contains(y))
			c->Cy = renderedChunks[y];
		if (renderedChunks.contains(y1))
			c->Cy1 = renderedChunks[y1];
		if (renderedChunks.contains(z))
			c->Cz = renderedChunks[z];
		if (renderedChunks.contains(z1))
			c->Cz1 = renderedChunks[z1];
		
		c->generateVertices();
	}
}

void Game::World::update(double delta)
{

}

void Game::World::render()
{
	for (auto& chunk : renderedChunks)
	{
		chunk.second->draw(localPlayer->camera.view, localPlayer->camera.proj);
	}
}

void Game::World::loadChunks()
{

	// ideas:
	// start with knowing player position.
	// see all chunks that need to be created within the sphere of the player
		// make sure these chunks are within the confines of the world (int32 x, y. uint8 z)
	// we need to have a queue system to load chunks.

	// old code
	int j = 0;
	for (int x = -3; x < 3; x++)
	{
		for (int y = -3; y < 3; y++)
		{
			for (int z = 3; z < 6; z++)
			{
				ChunkPos pos = { x, y, z };
				int t = Chunk::getChunkId(pos);
				renderedChunks[t] = new Chunk(texture, pos, &chunkVBO);
				renderedChunks[t]->generateTerrain(terrain.getHeightMap(pos));
				if (!renderedChunks[t]->isEmpty())
				{
					renderedChunks[t]->generateCaves(terrain.getCaves(pos));
				}
			}
		}
	}
}

void Game::World::renderChunks()
{
}

