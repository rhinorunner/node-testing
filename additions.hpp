#ifndef ADDITIONS_HPP_CHECK
#define ADDITIONS_HPP_CHECK

#include <iostream>
#include <vector>
#include <SDL.h>
#include "render.hpp"

static SDL_Window*   N_WINDOW;
static SDL_Renderer* N_RENDERER;

void N_SDLSETUP() {// the window
	N_WINDOW = NULL;
	// renderer
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	SDL_CreateWindowAndRenderer(N_SCREENWIDTH,N_SCREENHEIGHT,0,&N_WINDOW,&N_RENDERER);
	if (N_WINDOW == NULL) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		exit(1);
	}

	// set background color
	SDL_SetRenderDrawColor(N_RENDERER,N_BACKCOLOR.R,N_BACKCOLOR.G,N_BACKCOLOR.B,255);
	SDL_RenderClear(N_RENDERER);
}

void resetNodes(BetterRand& rand) 
{
	N_NODES = {};
	for (uint16_t i = 0; i < N_NODEAMOUNT; ++i) {
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
}

void calcNodes() 
{
	nodeConnections = {
		IMP_shortestPath(N_NODES),
		IMP_shortestPath(N_NODES,2),
		IMP_shortestPath(N_NODES,3),
		IMP_shortestPath(N_NODES,4),
		IMP_shortestPath(N_NODES,5),
		IMP_shortestPath(N_NODES,6)
	};

	// find the shortest node path
	float shortestPath = UINT16_MAX;
	uint16_t shortestPathName = 0;
	
	for (uint16_t ll = 0; ll < nodeConnections.size(); ++ll) {
		if (nodeDistanceTotal(nodeConnections[ll]) < shortestPath) {
			shortestPath = nodeDistanceTotal(nodeConnections[ll]);
			shortestPathName = ll;
		}
	}
	
	std::cout << "\n[" << N_NODES.size() << " nodes]\n";
	for (uint16_t im = 0; im < nodeConnections.size(); ++im) {
		std::cout 
			<< "Path " 
			<< im << ": " 
			<< nodeDistanceTotal(nodeConnections[im])
			<< (im == shortestPathName ? " <---" : "")
			<< '\n';
	}
	std::cout << '\n';
}

#endif