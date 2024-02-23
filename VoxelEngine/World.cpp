#include "World.h"
#include <array>
#include "VBO.h"

int getChunkId(int x, int y, int z) {
	return x + (CHUNK_RENDER_DISTANCE + 1) * (y + (CHUNK_RENDER_DISTANCE + 1) * z);
};

Game::World::World(Player* localPlayer) :
	localPlayer(localPlayer),
	texture("resources/texturepack-simple.jpg"),
	chunkVBO()
{
	for (int x = 0; x < 2; x++)
	{
		int t = getChunkId(x, 0, 0);
		ChunkPos pos = { x, 0, 0 };
		renderedChunks[t] = new Chunk(texture, pos, &chunkVBO);
	}
	for (auto& chunk : renderedChunks)
	{
		Chunk* c = chunk.second;

		int x  = getChunkId(c->chunkPos.X - 1, c->chunkPos.Y, c->chunkPos.Z);
		int x1 = getChunkId(c->chunkPos.X + 1, c->chunkPos.Y, c->chunkPos.Z);
		int y  = getChunkId(c->chunkPos.X, c->chunkPos.Y - 1, c->chunkPos.Z);
		int y1 = getChunkId(c->chunkPos.X, c->chunkPos.Y + 1, c->chunkPos.Z);
		int z  = getChunkId(c->chunkPos.X, c->chunkPos.Y, c->chunkPos.Z - 1);
		int z1 = getChunkId(c->chunkPos.X, c->chunkPos.Y, c->chunkPos.Z + 1);

		if (renderedChunks.contains(x))
			c->Cx  = renderedChunks[x];
		if (renderedChunks.contains(x1))
			c->Cx1 = renderedChunks[x1];
		if (renderedChunks.contains(y))
			c->Cy  = renderedChunks[y];
		if (renderedChunks.contains(y1))
			c->Cy1 = renderedChunks[y1];
		if (renderedChunks.contains(z))
			c->Cz  = renderedChunks[z];
		if (renderedChunks.contains(z1))
			c->Cz1 = renderedChunks[z1];
	}
	for (auto& chunk : renderedChunks)
	{
		chunk.second->generate();
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

void Game::World::load()
{
	// ideas:
	// start with knowing player position.
	// see all chunks that need to be created within the sphere of the player
		// make sure these chunks are within the confines of the world (int32 x, y. int8 z)
}

