#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "Player.h"
#include "Chunk.h"
#include "TextureArray.h"
#include "Terrain.h"
#include <map> 
#include <queue>
#include "ThreadSafeQueue.h"

#define CHUNK_RENDER_DISTANCE 4 // radius

namespace Game
{
	class World
	{
		std::vector<Chunk> modifiedChunks;
		std::unordered_map<ChunkPos, Chunk*, std::hash<ChunkPos>> renderedChunks;
		std::unordered_map<ChunkPos, Chunk*, std::hash<ChunkPos>> newChunks; // cache chunks that need to be rendered
		Game::Util::ThreadSafeQueue<Chunk*> qChunksRender; // queued chunks that need to be rendered
		Engine::GFX::TextureArray texture;
		ChunkPos lastPlayerPos;
		Player* localPlayer;
		Engine::GFX::VBO chunkVBO;
		Terrain terrain;

		void loadChunks();
		void renderChunks();
		Chunk* loadChunkAt(const ChunkPos& chunkPos);
	public:
		World(Player* localPlayer);
		void update(double delta);
		void render();
		void load();

		uint16_t getBlockId(const ChunkPos& chunkPos, int x, int y, int z);
	};
}
#endif