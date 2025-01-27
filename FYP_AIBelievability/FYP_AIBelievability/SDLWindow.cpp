#include "SDLWindow.h"
#include "Agent.h"
#include "Grid.h"
#include "Commons.h"

#include <iostream>
#include <vector>
#include <SDL_image.h>

SDLWindow::SDLWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) //initialise SDL
	{
		std::string error{ "Unable to initialise SDL: ", SDL_GetError() };
		throw std::runtime_error(error);
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::string error{ "Error SDL2_image Initialization", IMG_GetError() };
		throw std::runtime_error(error);
	}

	//Create window
	window = SDL_CreateWindow("WFC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_OPENGL); //name, horizontal pos, vertical pos, width, height, mode
	if (!window) {
		std::string error{ "Error window creation", SDL_GetError() };
		throw std::runtime_error(error);
	}

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //window to render to, index of rendering driver (-1 = first available), mode
	if (!renderer) {
		std::string error{ "Failed to create SDL Renderer: ", SDL_GetError() };
		throw std::runtime_error(error);
	}
}

SDLWindow::~SDLWindow()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void SDLWindow::BeginRender()
{
	SDL_RenderClear(renderer); //remove anything already rendered
}

void SDLWindow::EndRender()
{
	SDL_RenderPresent(renderer); //moving from back buffer to screen
}

bool SDLWindow::Events(Grid* grid, std::vector<Agent>& agents)
{
	SDL_Event e;
	if (SDL_PollEvent(&e))
	{
		ImGui_Implementation::ProcessEvents(&e);

		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if (e.type == SDL_MOUSEWHEEL)
		{
			for (Agent& a : agents)
			{
				a.needs.hungerVal += 10;
				a.needs.thirstVal += 10;
			}
		}
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(window))
		{
			return false;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
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
					ImGui_Implementation::OCEANValues = a.personalityComponent.OCEANValues;
					ImGui_Implementation::Traits = a.personalityComponent.traits;
					ImGui_Implementation::needStruct = a.needs;

					break;
				}
			}

			//CLICKING TILES
			for (std::vector<Tile>& v : grid->Tiles) //gets vectors of tiles
			{
				for (Tile& t : v) // tiles in vector
				{
					if (WFC::IsInTile(mousePos, t)) //if tile is clicked
					{
						for (Agent& a : agents)
						{
							if (ImGui_Implementation::toSelectDest && ImGui_Implementation::agentCount == a.agentCount)
							{
								a.GetStates().moveState.from = a.position;
								a.GetStates().moveState.to = t.GetGridPos();
								a.GetStates().moveState.isMoveToSet = true;

								ImGui_Implementation::toSelectDest = false;
							}
						}
					}
				}
			}
		}
	}

	return true;
}
