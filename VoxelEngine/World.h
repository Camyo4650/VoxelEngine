#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "Player.h"
#include "Chunk.h"
#include "TextureArray.h"
#include "Terrain.h"
#include <map> 
#include <queue>

#define CHUNK_RENDER_DISTANCE 1 // radius

namespace Game
{
	class World
	{
		std::vector<Chunk> modifiedChunks;
		std::map<int, Chunk*> renderedChunks; // cache chunks that are within range of the player.
		std::map<int, Chunk*> newChunks; // cache chunks that need to be rendered
		std::queue<Chunk*> qChunks; // queued chunks that need to be rendered
		Engine::GFX::TextureArray texture;
		Player* localPlayer;
		Engine::GFX::VBO chunkVBO;
		Terrain terrain;

		void loadChunks();
		void renderChunks();
	public:
		World(Player* localPlayer);
		void update(double delta);
		void render();
	};
}
#endif