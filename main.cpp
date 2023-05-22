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
	srand(clock());
	bool doneDrawing = false;

	N_RENDER Renderer {N_WINDOW, N_RENDERER};

	resetNodes();
	calcNodes();

	while (looping)
	{
		timeNew = clock();
		deltaTime = timeNew - timeOld;

		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) looping = false;
		else if (event.type == SDL_MOUSEMOTION) {
			mouseX = event.motion.x;
			mouseY = event.motion.y;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			// add a node at that location
			Node temp {
				mouseX,
				mouseY,
				(uint16_t)N_NODES.size(),
				10,
				false,
				{155,155,155}
			};
			N_NODES.push_back(temp);
			N_NODEAMOUNT++;
			calcNodes();
		}
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			// quit
			case SDLK_ESCAPE:
				looping = false;
				break;
			
			// reset nodes
			case SDLK_KP_ENTER:
				resetNodes();
				calcNodes();
				doneDrawing = false;
				break;

			// calc nodes no reset
			// reset nodes
			case SDLK_KP_PERIOD:
				calcNodes();
				doneDrawing = false;
				break;
			
			// increase node amount
			// also reset nodes
			case SDLK_KP_PLUS:
				if (N_NODEAMOUNT + N_NODESTEP > UINT16_MAX) {
					N_NODEAMOUNT = UINT16_MAX;
				}
				else N_NODEAMOUNT += N_NODESTEP;
				resetNodes();
				calcNodes();
				doneDrawing = false;
				break;
			
			// decrease node amount
			// also reset nodes
			case SDLK_KP_MINUS:
				if (N_NODEAMOUNT - N_NODESTEP < 1) {
					N_NODEAMOUNT = 2;
				}
				else N_NODEAMOUNT -= N_NODESTEP;
				resetNodes();
				calcNodes();
				doneDrawing = false;
				break;
			
			// increase line delay
			// reset nodes
			case SDLK_KP_9:
				N_NODEDELAY += N_NODEDELAYSTEP;
				resetNodes();
				calcNodes();
				doneDrawing = false;
				break;
			
			// decrease line delay
			// reset nodes
			case SDLK_KP_6:
				if (N_NODEDELAY - N_NODEDELAYSTEP < 0) N_NODEDELAY = 0;
				else N_NODEDELAY -= N_NODEDELAYSTEP;
				resetNodes();
				calcNodes();
				doneDrawing = false;
				break;

			// clear all nodes
			case SDLK_KP_0:
				N_NODES = {};
				N_NODEAMOUNT = 0;
				resetNodes();
				calcNodes();
				break;
			
			case SDLK_1: if (nodeShow.size() >= 1)  nodeShow[0] = !nodeShow[0]; break;
			case SDLK_2: if (nodeShow.size() >= 2)  nodeShow[1] = !nodeShow[1]; break;
			case SDLK_3: if (nodeShow.size() >= 3)  nodeShow[2] = !nodeShow[2]; break;
			case SDLK_4: if (nodeShow.size() >= 4)  nodeShow[3] = !nodeShow[3]; break;
			case SDLK_5: if (nodeShow.size() >= 5)  nodeShow[4] = !nodeShow[4]; break;
			case SDLK_6: if (nodeShow.size() >= 6)  nodeShow[5] = !nodeShow[5]; break;
			case SDLK_7: if (nodeShow.size() >= 7)  nodeShow[6] = !nodeShow[6]; break;
			case SDLK_8: if (nodeShow.size() >= 8)  nodeShow[7] = !nodeShow[7]; break;
			case SDLK_9: if (nodeShow.size() >= 9)  nodeShow[8] = !nodeShow[8]; break;
			case SDLK_0: if (nodeShow.size() >= 10) nodeShow[9] = !nodeShow[9]; break;
			}
		}
		
		// render nodes
		for (Node i : N_NODES) {
			Renderer.blitCircle(
				i.X,
				i.Y,i.radius,
				{i.color.R, i.color.G, i.color.B}
			);
		}

		// render lines connecting the nodes
		uint16_t tempC = 0;
		for (uint16_t B = 0; B < nodeConnections.size(); ++B) {
			if (tempC >= Colors::list.size()) tempC = 0;
			if (nodeShow[B]) {
				for (uint16_t j = 0; j < nodeConnections[B].size(); ++j) {
					// make sure the node isnt the last element
					if (j == (nodeConnections[B].size() - 1)) break;

					uint16_t nodeIndex = findId(nodeConnections[B][j]);

					Node temp1 = N_NODES[findId(nodeConnections[B][j])];
					Node temp2 = N_NODES[findId(nodeConnections[B][j+1])];

					float offset = B;

					Renderer.blitLine(
						{temp1.X+offset, temp1.Y+offset},
						{temp2.X+offset, temp2.Y+offset},
						Colors::list[tempC],
						1
					);
					if (N_NODEDELAY && !doneDrawing) {
						Sleep(N_NODEDELAY);
						SDL_RenderPresent(N_RENDERER);
					}
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