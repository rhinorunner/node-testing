#include <iostream>
#include <chrono>
#include <vector>
#include <SDL.h>
#undef main
#include "render.hpp"

int main()
{
	// the window
	SDL_Window* N_WINDOW = NULL;
	// renderer
	SDL_Renderer* N_RENDERER;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_CreateWindowAndRenderer(N_SCREENWIDTH, N_SCREENHEIGHT, 0, &N_WINDOW, &N_RENDERER);
	if (N_WINDOW == NULL) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// set background color
	SDL_SetRenderDrawColor(N_RENDERER, N_BACKCOLOR.R, N_BACKCOLOR.G, N_BACKCOLOR.B, 255);
	SDL_RenderClear(N_RENDERER);

	SDL_Event event;
	bool looping = true;
	uint16_t mouseX, mouseY;
	clock_t timeOld = clock();
	clock_t timeNew = clock();
	float deltaTime = 0;

	N_RENDER Renderer {N_WINDOW, N_RENDERER};

	while (looping)
	{
		timeNew = clock();
		deltaTime = timeNew - timeOld;

		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT) looping = false;
		if (event.type == SDL_MOUSEMOTION) {
		SDL_MOUSEMOTION:
			mouseX = event.motion.x;
			mouseY = event.motion.y;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				looping = false;
				break;
			}
		}

		Renderer.blitLine({100,100},{150,150},Colors::white);
		Renderer.blitCircle(300,300,30,Colors::green);
		SDL_SetRenderDrawColor(N_RENDERER, 255,255,0, 255);
		Renderer.DrawCircle(N_RENDERER,200,200,30);

		// render stuff:
		SDL_RenderPresent(N_RENDERER);
		// clear screen
		SDL_SetRenderDrawColor(N_RENDERER, N_BACKCOLOR.R, N_BACKCOLOR.G, N_BACKCOLOR.B, 255);
		SDL_RenderClear(N_RENDERER);

		// reset deltatime
		timeOld = timeNew;
	}

	SDL_DestroyWindow(N_WINDOW);
	SDL_Quit();

	return 0;
}