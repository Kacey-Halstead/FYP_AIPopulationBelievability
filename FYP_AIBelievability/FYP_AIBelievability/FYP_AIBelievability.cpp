#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include <array>
#include<algorithm>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#include "WFC.h"

struct SDLVars
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};

SDLVars* InitSDL()
{
	SDLVars* sdlVars{};

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) //initialise SDL
	{
		std::cout << "Unable to initialise SDL: " << SDL_GetError();
		return nullptr;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		return nullptr;
	}

	//Create window
	sdlVars->window = SDL_CreateWindow("WFC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL); //name, horizontal pos, vertical pos, width, height, mode
	if (sdlVars->window == NULL) {
		std::cout << "Error window creation";
		return nullptr;
	}

	//Create renderer
	sdlVars->renderer = SDL_CreateRenderer(sdlVars->window, -1, SDL_RENDERER_ACCELERATED); //window to render to, index of rendering driver (-1 = first available), mode
	if (sdlVars->renderer == NULL) {
		std::cout << "Error renderer creation";
		return nullptr;
	}
}

int main(int argc, char* argv[])
{
	SDLVars* sdlVars = InitSDL();
	if (sdlVars == nullptr) return 0; //if init error

	srand(time(NULL));
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//WFC Init
	WFC* WFCComponent = new WFC(10, 10, sdlVars->window, sdlVars->renderer);
	WFCComponent->WFCBody();
	WFCComponent->CreateRects();

	//main loop
	while (true)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e)) //ends loop if exit button pressed
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
		}

		SDL_RenderClear(sdlVars->renderer); //remove anything already rendered
		WFCComponent->RenderWFC(sdlVars->renderer); //render WFC 
		SDL_RenderPresent(sdlVars->renderer); //moving from back buffer to screen
	}

	SDL_DestroyRenderer(sdlVars->renderer);
	SDL_DestroyWindow(sdlVars->window);
	delete WFCComponent;
	IMG_Quit();
	SDL_Quit();

	return 0;
}
