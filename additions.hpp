#ifndef ADDITIONS_HPP_CHECK
#define ADDITIONS_HPP_CHECK

#include <iostream>
#include <vector>
#include <SDL.h>
#include <thread>
#include "render.hpp"

static SDL_Window*   N_WINDOW;
static SDL_Renderer* N_RENDERER;

// window setup
void N_SDLSETUP() { 
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

void resetNodes() {
	N_NODES = {};
	for (uint16_t i = 0; i < N_NODEAMOUNT; ++i) {
		Node temp {
			(rand() % N_SCREENWIDTH),
			(rand() % N_SCREENHEIGHT),
			i,
			10,
			false,
			{155,155,155}
		};
		N_NODES.push_back(temp);
	}
}

void calcNodes() {
	bool first = false;
	
	nodeConnections = {
		IMP_shortestPath(N_NODES),
		IMP_shortestPath_branched(N_NODES,3,3,0,0)
	};

	if (nodeShow.empty()) {
		first = true;
		for (auto i : nodeConnections) nodeShow.push_back(true);
	}

	// find the shortest node path
	float shortestPath = FLT_MAX;
	uint16_t shortestPathName = 0;
	
	for (uint16_t ll = 0; ll < nodeConnections.size(); ++ll) {
		if ((nodeDistanceTotal(nodeConnections[ll]) < shortestPath) && nodeShow[ll]) {
			shortestPath = nodeDistanceTotal(nodeConnections[ll]);
			shortestPathName = ll;
		}
	}
	
	std::cout << "\n[" << N_NODES.size() << " nodes]\n";
	for (uint16_t im = 0; im < nodeConnections.size(); ++im) {
		std::cout 
			<< (nodeShow[im] ? "[shown]  " : "[hidden] ")
			<< "Path " 
			<< im << ": " 
			<< nodeDistanceTotal(nodeConnections[im])
			<< (im == shortestPathName ? " <---" : "")
			<< '\n';
	}
	std::cout << '\n';
}

#endif