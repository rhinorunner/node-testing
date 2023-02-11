#include <iostream>
#include <chrono>
#include <vector>
#include <SDL.h>
#undef main
#include "render.hpp"
#include "implementations.hpp"

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
	BetterRand rand {};

	N_RENDER Renderer {N_WINDOW, N_RENDERER};

	for (uint16_t i = 0; i < 10; ++i) {
		Node temp {
			(rand.genRand() % N_SCREENWIDTH),
			(rand.genRand() % N_SCREENHEIGHT),
			i,
			false,
			{
				(uint8_t)(rand.genRand() % 70)+160,
				(uint8_t)(rand.genRand() % 70)+160,
				(uint8_t)(rand.genRand() % 70)
			},
			(rand.genRand() % 5)+5
		};
		N_NODES.push_back(temp);
	}

	// holds the node connection arrays, in order of connections
	static std::vector<std::vector<uint16_t>> nodeConnections {};

	nodeConnections.push_back(IMP_shortestPath(N_NODES));
	
	for (auto i : nodeConnections) {
		for (auto j : i) {
			std::cout << j << " ";
		}
		std::cout << std::endl;
	}

	while (looping)
	{
		// UPDATE VARS
		
		timeNew = clock();
		deltaTime = timeNew - timeOld;

		// USER INPUT

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

		// DRAW THINGS
		
		for (auto i : N_NODES) {
			Renderer.blitCircle(
				i.X,
				i.Y,i.radius,
				{i.color.R, i.color.G, i.color.B}
			);
		}
		// render lines connecting the nodes
		for (uint16_t j = 0; j < nodeConnections[0].size(); ++j) {
			// make sure the node isnt the last element
			if (j == (nodeConnections[0].size() - 1)) break;
			Renderer.blitLine(
				{N_NODES[nodeConnections[0][j    ]].X, N_NODES[nodeConnections[0][j    ]].Y},
				{N_NODES[nodeConnections[0][j + 1]].X, N_NODES[nodeConnections[0][j + 1]].Y},
				{255,255,255}
			);
		}

		// FINISH RENDER
	
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