#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <SDL_image.h>

#include "ImGuiImplementation.h"
#include "Agent.h"
#include "WFC.h"
#include "Grid.h"

class SDLWindow
{
public:
	SDLWindow();
	~SDLWindow();

	bool InitSDL();
	void DestroySDL();

	void BeginRender();
	void EndRender();
	bool Events(Grid* grid, std::vector<Agent>& agents);

	inline SDL_Window* getWindow() { return window; }
	inline SDL_Renderer* getRenderer() { return renderer; }


private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};

