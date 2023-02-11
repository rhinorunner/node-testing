#ifndef RENDER_HPP_CHECK
#define RENDER_HPP_CHECK

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <SDL.h>
#undef main

#pragma pack(1)

struct RGB_t {
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

struct RGBI_t {
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t I;
};

/*********************************************/

// screen width
static constexpr uint16_t N_SCREENWIDTH = 500;
// screen height
static constexpr uint16_t N_SCREENHEIGHT = 500;

// background color
static const RGB_t N_BACKCOLOR = { 0,0,0 };

/*********************************************/

struct Node {
	uint16_t X;
	uint16_t Y;
	uint32_t id;
	bool check;
};

// stores all nodes
static std::vector<Node> R_NODES {};

class N_RENDER {
private:
	SDL_Window* Window;
	SDL_Renderer* Renderer;

public:
	N_RENDER(SDL_Window* window, SDL_Renderer* renderer)
		: Window(window), Renderer(renderer) {}

	void blitPixel(
		const uint16_t& X,
		const uint16_t& Y, 
		const RGB_t& color
	) {
		SDL_SetRenderDrawColor(Renderer, color.R, color.G, color.B, 255);
		SDL_RenderDrawPoint(Renderer, X, Y);
	}

	void blitLine(
		const std::pair<uint16_t, uint16_t>& coords1,
		const std::pair<uint16_t, uint16_t>& coords2,
		const RGB_t& color
	) {
		SDL_SetRenderDrawColor(
			Renderer,
			color.R,
			color.G,
			color.B,
			255
		);
		SDL_RenderDrawLine(
			Renderer,
			coords1.first,
			coords1.second,
			coords2.first,
			coords2.second
		);
	}

	void blitCircle(
		const uint16_t& centerX,
		const uint16_t& centerY, 
		const uint16_t& radius,
		const RGB_t& color
	) {
		const uint16_t diameter = (radius * 2);

		uint16_t x = (radius - 1);
		uint16_t y = 0;
		uint16_t tx = 1;
		uint16_t ty = 1;
		uint16_t error = (tx - diameter);

		SDL_SetRenderDrawColor(Renderer,color.R,color.B,color.G,255);

		while (x >= y) {
			// Each of the following renders an octant of the circle
			SDL_RenderDrawPoint(Renderer, centerX + x, centerY - y);
			SDL_RenderDrawPoint(Renderer, centerX + x, centerY + y);
			SDL_RenderDrawPoint(Renderer, centerX - x, centerY - y);
			SDL_RenderDrawPoint(Renderer, centerX - x, centerY + y);
			SDL_RenderDrawPoint(Renderer, centerX + y, centerY - x);
			SDL_RenderDrawPoint(Renderer, centerX + y, centerY + x);
			SDL_RenderDrawPoint(Renderer, centerX - y, centerY - x);
			SDL_RenderDrawPoint(Renderer, centerX - y, centerY + x);

			if (error <= 0) {
  				++y;
  				error += ty;
  				ty += 2;
			}
			if (error > 0) {
  				--x;
  				tx += 2;
  				error += (tx - diameter);
			}
		}
	}
	void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
	{
	   const int32_t diameter = (radius * 2);

	   int32_t x = (radius - 1);
	   int32_t y = 0;
	   int32_t tx = 1;
	   int32_t ty = 1;
	   int32_t error = (tx - diameter);

	   while (x >= y)
	   {
		  //  Each of the following renders an octant of the circle
		  SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		  SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		  SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		  SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		  SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		  SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		  SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		  SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		  if (error <= 0)
		  {
			 ++y;
			 error += ty;
			 ty += 2;
		  }

		  if (error > 0)
		  {
			 --x;
			 tx += 2;
			 error += (tx - diameter);
		  }
	   }
	}
};

namespace Colors 
{
	RGB_t white  = {255,255,255};
	RGB_t black  = {0  ,0  ,0  };
	RGB_t red    = {255,0  ,0  };
	RGB_t green  = {0  ,255,0  };
	RGB_t blue   = {0  ,0  ,255};
	RGB_t yellow = {255,255,0  };
	RGB_t purple = {255,0  ,255};
	RGB_t cyan   = {0  ,255,255};
};

#endif