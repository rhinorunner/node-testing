#include <iostream>
#include <chrono>
#include <vector>
#include <windows.h>
#include <SDL.h>
#undef main
#include "render.hpp"
#include "implementations.hpp"
#include "additions.hpp"



int main()
{
	N_SDLSETUP();

	SDL_Event event;
	bool looping = true;
	uint16_t mouseX, mouseY;
	clock_t timeOld = clock();
	clock_t timeNew = clock();
	float deltaTime = 0;
	BetterRand rand {};
	bool doneDrawing = false;

	N_RENDER Renderer {N_WINDOW, N_RENDERER};

	resetNodes(rand);
	calcNodes();

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
			// quit
			case SDLK_ESCAPE:
				looping = false;
				break;
			
			// reset nodes
			case SDLK_KP_ENTER:
				resetNodes(rand);
				calcNodes();
				doneDrawing = false;
				break;
			
			// increase node amount
			// also reset nodes
			case SDLK_KP_PLUS:
				if (N_NODEAMOUNT + N_NODESTEP > UINT16_MAX) {
					N_NODEAMOUNT = UINT16_MAX;
				}
				N_NODEAMOUNT += N_NODESTEP;
				resetNodes(rand);
				calcNodes();
				doneDrawing = false;
				break;
			
			// decrease node amount
			// also reset nodes
			case SDLK_KP_MINUS:
				if (N_NODEAMOUNT - N_NODESTEP < 1) {
					N_NODEAMOUNT = 2;
				}
				N_NODEAMOUNT -= N_NODESTEP;
				resetNodes(rand);
				calcNodes();
				doneDrawing = false;
				break;
			}
		}
		
		for (auto i : N_NODES) {
			Renderer.blitCircle(
				i.X,
				i.Y,i.radius,
				{i.color.R, i.color.G, i.color.B}
			);
		}
		
		// render lines connecting the nodes
		uint16_t tempC = 0;
		for (uint16_t B = 0; B < nodeConnections.size(); ++B) 
		{
			if (tempC >= Colors::list.size()) tempC = 0;
			for (uint16_t j = 0; j < nodeConnections[B].size(); ++j) 
			{
				// make sure the node isnt the last element
				if (j == (nodeConnections[B].size() - 1)) break;

				uint16_t nodeIndex = findId(nodeConnections[0][j]);

				Node temp1 = N_NODES[findId(nodeConnections[0][j  ])];
				Node temp2 = N_NODES[findId(nodeConnections[0][j+1])];

				Renderer.blitLine(
					{temp1.X, temp1.Y},
					{temp2.X, temp2.Y},
					Colors::list[tempC],
					1
				);
				if (N_NODEDELAY && !doneDrawing) {
					Sleep(N_NODEDELAY);
					SDL_RenderPresent(N_RENDERER);
				}
			}
			tempC++;
		}
		doneDrawing = true;
		
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