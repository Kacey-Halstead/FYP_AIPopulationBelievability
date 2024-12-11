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


int InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) //initialise SDL
	{
		std::cout << "Unable to initialise SDL: " << SDL_GetError();
		return 1;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		return 2;
	}
}

int main(int argc, char* argv[])
{
	InitSDL();

	//Create window
	SDL_Window* window = SDL_CreateWindow("WFC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL); //name, horizontal pos, vertical pos, width, height, mode
	if (window == NULL) {
		std::cout << "Error window creation";
		return 3;
	}

	//Create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //window to render to, index of rendering driver (-1 = first available), mode
	if (renderer == NULL) {
		std::cout << "Error renderer creation";
		return 4;
	}

	WFC* WFCComponent = new WFC(10, 10, window, renderer);

	srand(time(NULL));
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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

		SDL_RenderClear(renderer); //remove anything already rendered
		//TODO Add WFC rendering func here
		SDL_RenderPresent(renderer); //moving from back buffer to screen
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
