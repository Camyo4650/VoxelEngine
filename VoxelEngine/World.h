#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "Player.h"
#include "Chunk.h"
#include "TextureArray.h"
#include <map> 

#define CHUNK_RENDER_DISTANCE 8 // radius

namespace Game
{
	class World
	{
		std::vector<Chunk> modifiedChunks;
		std::map<int, Chunk*> renderedChunks;
		Engine::GFX::TextureArray texture;
		Player* localPlayer;
		void load();
		Engine::GFX::VBO chunkVBO;
	public:
		World(Player* localPlayer);
		void update(double delta);
		void render();
	};
}
#endif