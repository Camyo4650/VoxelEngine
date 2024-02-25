#pragma once

#include <SDL/SDL.h>
#include <vector>
#include "World.h"
#include "Player.h"

namespace Engine {
	class GameEngine
	{
		Game::World world;
		Game::Player player;
		//Engine::GFX::Mesh axis; // later we can move this to a construct called GUI
	public:
		GameEngine(int w, int h);
		void input(SDL_Event* event);
		void update(double delta);
		void render();
		~GameEngine();
	};
}