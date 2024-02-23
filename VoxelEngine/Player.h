#pragma once

#include <SDL/SDL.h>
#include "Camera.h"
#include "Chunk.h"

namespace Game {
	class Player
	{
		bool move[4];
		double time;
	public:
		ChunkPos chunkPos;
		glm::vec3 pos;

		GFX::Camera camera;
		Player();
		void input(SDL_Event* e);
		void update(double delta);
		void render();
	};
}