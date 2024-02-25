#include "GameEngine.h"

#include <glad/glad.h>
#include <SDL/SDL.h>
#include "AxisVertex.h"

bool focused = true;

Engine::GameEngine::GameEngine(int w, int h) :
	player(w, h),
	world(&this->player)
	/*
	axis(Engine::GFX::Shader("axis.v.glsl", "axis.f.glsl"),
	{
		{3, GL_FLOAT,	GL_FALSE, sizeof(Game::GFX::AxisVertex), 0},
		{1, GL_INT,		GL_FALSE, sizeof(Game::GFX::AxisVertex), (void*)(3 * sizeof(float))}
	}, new Engine::GFX::VBO())*/
{
}

void Engine::GameEngine::input(SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
			focused = false;
		}
	}
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (!focused)
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			focused = true;
		}
	}
	this->player.input(event);
}

void Engine::GameEngine::update(double delta)
{
	this->player.update(delta);
}

void Engine::GameEngine::render()
{
	glCall(glClearColor(0.1f, 0.5f, 0.6f, 1.0f));
	glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	world.render();
}

Engine::GameEngine::~GameEngine()
{

}
