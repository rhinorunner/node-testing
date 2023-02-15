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

struct Node {
	uint16_t X;
	uint16_t Y;
	uint16_t id;
	bool check;
	RGB_t color = {255,255,255};
	uint16_t radius = 10;
};

/*********************************************/

// screen width
static constexpr uint16_t N_SCREENWIDTH = 900;
// screen height
static constexpr uint16_t N_SCREENHEIGHT = 900;

// amount of nodes
static uint16_t N_NODEAMOUNT = 100;
// how much to increase/decrease nodes by
static constexpr uint16_t N_NODESTEP = 10;
// node line delay, in ms
static uint16_t N_NODEDELAY = 2;
// node delay increase/decrease step
static uint16_t N_NODEDELAYSTEP = 2;

// background color
static const RGB_t N_BACKCOLOR = {0,0,0};

// stores all nodes
static std::vector<Node> N_NODES {};

// holds the node connection arrays, in order of connections
static std::vector<std::vector<uint16_t>> nodeConnections {};

/*********************************************/

float distance(
	const std::pair<uint16_t,uint16_t>& coords1,
	const std::pair<uint16_t,uint16_t>& coords2
) {
	return sqrt(
		pow(coords1.first  - coords2.first, 2) +
		pow(coords1.second - coords2.second, 2)
	);
}

class N_RENDER {
private:
	SDL_Window* Window;
	SDL_Renderer* Renderer;

public:
	N_RENDER(SDL_Window* window, SDL_Renderer* renderer)
		: Window(window), Renderer(renderer) {}

	// blit a pixel
	void blitPixel(
		const uint16_t& X,
		const uint16_t& Y, 
		const RGB_t& color
	) {
		SDL_SetRenderDrawColor(Renderer, color.R, color.G, color.B, 255);
		SDL_RenderDrawPoint(Renderer, X, Y);
	}

	// blit a line from coords1 to coords2
	void blitLine(
		const std::pair<uint16_t, uint16_t>& coords1,
		const std::pair<uint16_t, uint16_t>& coords2,
		const RGB_t& color,
		uint16_t thickness = 1
	) {
		SDL_SetRenderDrawColor(
			Renderer,
			color.R,
			color.G,
			color.B,
			255
		);
		for (uint16_t i = 0; i < thickness;++i) {
			SDL_RenderDrawLine(
				Renderer,
				coords1.first+i,
				coords1.second+i,
				coords2.first+i,
				coords2.second+i
			);
		}
	}

	// blit a circle (no fill)
	void blitCircle(
		const uint16_t& centerX, 
		const uint16_t& centerY, 
		const uint16_t& radius,
		const RGB_t& color
	)
	{
		const int32_t diameter = (radius * 2);

		int16_t x = (radius - 1);
		int16_t y = 0, tx = 1, ty = 1;
		int16_t error = (tx - diameter);

		SDL_SetRenderDrawColor(Renderer,color.R,color.G,color.B,255);

		while (x >= y)
		{
			// each of the following renders an octant of the circle
			SDL_RenderDrawPoint(Renderer, centerX + x, centerY - y);
			SDL_RenderDrawPoint(Renderer, centerX + x, centerY + y);
			SDL_RenderDrawPoint(Renderer, centerX - x, centerY - y);
			SDL_RenderDrawPoint(Renderer, centerX - x, centerY + y);
			SDL_RenderDrawPoint(Renderer, centerX + y, centerY - x);
			SDL_RenderDrawPoint(Renderer, centerX + y, centerY + x);
			SDL_RenderDrawPoint(Renderer, centerX - y, centerY - x);
			SDL_RenderDrawPoint(Renderer, centerX - y, centerY + x);

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

	// blit a circle (filled)
	// kind of shit, dont use this
	void blitCircleFull(
		const uint16_t& centerX,
		const uint16_t& centerY,
		const uint16_t& radius,
		const RGB_t& color
	) {
		for (uint16_t i = 0; i < radius; i++) {
			blitCircle(centerX,centerY,i,color);
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

	std::array<RGB_t,8> allColors = {
		white,black,red,green,blue,yellow,purple,cyan
	};

	std::array<RGB_t,8> list = {
		white,red,green,blue,yellow,purple,cyan
	};
};

class BetterRand {
public:
	// adds this to random each time, optional
	int32_t extraRand;
	BetterRand(const int32_t &ExtraRand = 0) : extraRand(ExtraRand){};
	uint32_t genRand(
		const int32_t &extra = 4, 
		bool resetExtraRand = true, 
		int32_t resetERextraIt = 2
	) {
		if (resetExtraRand)
		  extraRand = genRand(resetERextraIt, false);
		// set random to unix time
		auto cool = std::chrono::system_clock::now();
		auto very =
		    (unsigned int)
			std::chrono::time_point_cast<std::chrono::milliseconds>
			(cool).time_since_epoch().count();
		// add random()
		if (extra >= 1)
			very -= rand();
		// add line number
		if (extra >= 2)
			very += __LINE__;
		// add an iteration (extra = 2)
		if (extra >= 3)
			very += genRand(2, false);
		// bitshift right or left based on another iteration
		if (extra >= 4)
			(genRand(2, false)) % 2 ? very >>= 1 : very <<= 1;
		// subtract an iteration (extra = 4)		
		return (very + extraRand);
	}
};

#endif