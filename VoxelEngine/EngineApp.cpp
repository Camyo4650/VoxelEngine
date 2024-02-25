#include <glad/glad.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <iostream>

#include "../GameEngine.h"

#define SCR_W 1280
#define SCR_H 720

#define G_DEBUG

SDL_Window* G_GFX_APP_WINDOW = nullptr;
SDL_GLContext	G_OpenGL_CONTEXT;
bool G_QUIT = false;

namespace Engine
{
	GameEngine* gameEngine;
#ifdef G_DEBUG
	void D_OpenGLInfo()
	{
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	}
#endif

	void init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "Failed to initialize SDL video subsystem.\n";
			exit(1);
		}

		// Initialize SDL_image
		if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
			std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
			SDL_Quit();
			exit(1);
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_SetRelativeMouseMode(SDL_TRUE);

		G_GFX_APP_WINDOW = SDL_CreateWindow("VoxelEngine", 100, 100, SCR_W, SCR_H, SDL_WINDOW_OPENGL);

		if (G_GFX_APP_WINDOW == nullptr)
		{
			std::cout << "SDL_Window failed to initialize"
				<< std::endl;
			exit(1);
		}

		G_OpenGL_CONTEXT = SDL_GL_CreateContext(G_GFX_APP_WINDOW);

		if (G_GFX_APP_WINDOW == nullptr)
		{
			std::cout << "OpenGL context not available"
				<< std::endl;
			exit(1);
		}

		if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		{
			std::cout << "GLAD was not initialized"
				<< std::endl;
			exit(1);
		}

		D_OpenGLInfo();
		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glEnable(GL_CULL_FACE));

		gameEngine = new GameEngine(SCR_W, SCR_H);
	}

	void mainLoop()
	{
		double timeThen = SDL_GetTicks();
		while (!G_QUIT)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					G_QUIT = true;
				}
				else
				{
					gameEngine->input(&event);
				}
			}
			double timeNow = SDL_GetTicks();
			double deltaTime = timeNow - timeThen;
			timeThen = timeNow;
			gameEngine->update(deltaTime);
			gameEngine->render();

			SDL_GL_SwapWindow(G_GFX_APP_WINDOW);
		}
	}

	void terminate()
	{
		delete(gameEngine);

		SDL_DestroyWindow(G_GFX_APP_WINDOW);

		SDL_Quit();
	}
}