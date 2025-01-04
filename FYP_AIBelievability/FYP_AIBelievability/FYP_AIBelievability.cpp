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

#include "ImGuiImplementation.h"
#include "WFC.h"
#include "Agent.h"
#include "FromJSON.h"
#include "AStar.h"

struct InitVars
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool initFail = false;
};

std::vector<Agent> agents;


InitVars InitSDL()
{
	InitVars initVars{};

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) //initialise SDL
	{
		std::cout << "Unable to initialise SDL: " << SDL_GetError();
		initVars.initFail = true;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		initVars.initFail = true;
	}

	//Create window
	initVars.window = SDL_CreateWindow("WFC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL); //name, horizontal pos, vertical pos, width, height, mode
	if (initVars.window == NULL) {
		std::cout << "Error window creation";
		initVars.initFail = true;
	}

	//Create renderer
	initVars.renderer = SDL_CreateRenderer(initVars.window, -1, SDL_RENDERER_ACCELERATED); //window to render to, index of rendering driver (-1 = first available), mode
	if (initVars.renderer == NULL) {
		std::cout << "Error renderer creation";
		initVars.initFail = true;
	}

	srand(time(NULL));
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	return initVars;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	FromJSONFile::ReadFromJSON();

	InitVars returnVal = InitSDL();
	if (returnVal.initFail == true) return 0; //if init error
	InitVars* initVars = &returnVal;

	//TextureManager init
	if (!TextureManager::TextureManagerInit(initVars->renderer, initVars->window)) return 0; //if error loading textures

	//ImGui init
	ImGui_Implementation::Init(initVars->renderer, initVars->window);

	//GRID Init
	Grid* grid = new Grid(10, 10, { 'L', 'C', 'S' });

	//WFC Init
	WFC WFCComponent(grid);
	WFCComponent.WFCBody();
	WFCComponent.CreateRects(initVars->window);
	vector<vector<Tile*>> tiles = WFCComponent.GetTiles();

	//AStar
	AStar aStar = AStar(grid);

	//temp - for A* test
	Tile* start = nullptr;
	Tile* end = nullptr;

	agents.resize(10);

	//Agent init
	for (int i = 1; i < 11; i++)
	{
		ImGui_Implementation::agentCount = i;
		agents[i-1] = Agent();
	}
	
	//main loop
	while (true)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			ImGui_Implementation::ProcessEvents(&e);

			if (e.type == SDL_QUIT)
			{
				break;
			}
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(initVars->window))
			{
				break;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int padding = 50;
				int x = e.button.x;
				int y = e.button.y;

				SDL_Point mousePos = { x,y };

				//CLICKING AGENTS
				for (Agent& a : agents)
				{
					if (a.IsPointInAgent(mousePos))
					{
						ImGui_Implementation::isAgentPressed = true;
						ImGui_Implementation::agentCount = a.agentCount;
						ImGui_Implementation::OCEANValues = a.personality.OCEANValues;
						ImGui_Implementation::Traits = a.personality.traits;
						break;
					}
				}


				//CLICKING TILES
				for (std::vector<Tile*> v : WFCComponent.GetTiles()) //gets vectors of tiles
				{
					for (Tile* t : v) // tiles in vector
					{
						if (WFCComponent.IsInTile(mousePos, *t)) //if tile is clicked
						{
							if (start == nullptr) 
							{
								start = t;
							}
							else if (end == nullptr && t != start)
							{
								end = t;
							}


							if (start != nullptr && end != nullptr)
							{
								aStar.AllowDiagonal = false;
								aStar.ResetTiles(tiles);
								aStar.Findpath(start, end);
								start = nullptr;
								end = nullptr;
							}
						}
					}
				}
			}
		}

		//ImGui windows
		ImGui_Implementation::AgentPopUp();


		//RENDERING
		SDL_RenderClear(initVars->renderer); //remove anything already rendered

		WFCComponent.RenderWFC(initVars->renderer); //render WFC 

		for (Agent& a : agents) //render agents
		{
			a.Render(initVars->renderer, initVars->window);
		}

		ImGui_Implementation::ImGuiDraw(initVars->renderer);
		SDL_RenderPresent(initVars->renderer); //moving from back buffer to screen
	}

	// Cleanup
	ImGui_Implementation::Destroy();
	SDL_DestroyRenderer(initVars->renderer);
	SDL_DestroyWindow(initVars->window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
