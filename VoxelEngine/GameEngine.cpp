#include "GameEngine.h"

#include <glad/glad.h>
#include <SDL/SDL.h>
#include "AxisVertex.h"

bool focused = true;

Engine::GameEngine::GameEngine(int w, int h) :
	world(&this->player),
	player(w, h)
	/*
	axis(Engine::GFX::Shader("axis.v.glsl", "axis.f.glsl"),
	{
		{3, GL_FLOAT,	GL_FALSE, sizeof(Game::GFX::AxisVertex), 0},
		{1, GL_INT,		GL_FALSE, sizeof(Game::GFX::AxisVertex), (void*)(3 * sizeof(float))}
	}, new Engine::GFX::VBO())*/
{
	lastTime = SDL_GetTicks();
	frameCount = 0;
	world.load();
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
	this->world.update(delta);
}

void Engine::GameEngine::render()
{
	Uint32 currentTime = SDL_GetTicks();
	glCall(glClearColor(0.1f, 0.5f, 0.6f, 1.0f));
	glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	world.render();
	Uint32 newTime = SDL_GetTicks();
	if (currentTime >= lastTime + 1000)
	{
		printf("\r\033[KFPS:\t%6.1f", (float)frameCount / ((currentTime - (float)lastTime) / 1000)); // Use \r to overwrite the current line
		fflush(stdout);
		lastTime = currentTime;
		frameCount = 0;
		printf("\t\tRendered in %9.d", newTime - lastTime);
	}
	frameCount++;
}

Engine::GameEngine::~GameEngine()
{

}
