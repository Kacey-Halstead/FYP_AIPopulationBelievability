#pragma once
#include <SDL.h>
#include <vector>

class Agent;
class Grid;

class SDLWindow
{
public:
	SDLWindow();
	~SDLWindow();

	void BeginRender();
	void EndRender();
	bool Events(Grid* grid, std::vector<Agent>& agents);

	inline SDL_Window* getWindow() { return window; }
	inline SDL_Renderer* getRenderer() { return renderer; }


private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};

