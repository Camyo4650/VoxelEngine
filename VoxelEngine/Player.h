#pragma once

#include <SDL/SDL.h>
#include "Camera.h"
#include "Chunk.h"

namespace Game {
	class Player
	{
		bool move[6];
		double time;
	public:
		glm::vec3 pos;

		GFX::Camera camera;
		Player(int w, int h);
		void input(SDL_Event* e);
		void update(double delta);
		void render();

		ChunkPos getChunkCoords();
	};
}