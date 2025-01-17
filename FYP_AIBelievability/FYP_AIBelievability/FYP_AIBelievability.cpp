#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <array>
#include <algorithm>
#include <SDL.h>
#include <chrono>
#include <SDL_image.h>
#include <ctime>

#include "ImGuiImplementation.h"
#include "WFC.h"
#include "Agent.h"
#include "FromJSON.h"
#include "AStar.h"
#include "Commons.h"
#include "Action.h"

using namespace std::chrono;

std::vector<Agent> agents;

struct InitVars
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool initFail = false;
};

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


	//unsigned int seed = time(nullptr);
	unsigned int seed = 'k' + 'a' + 'c' + 'e' + 'y';
	srand(seed);

	FromJSONFile::ReadFromJSON();

	return initVars;
}

bool PollEvents(Grid* grid, WFC* WFCComponent, InitVars* initVars) 
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
			for (Agent a : agents)
			{
				a.needs.hungerVal += 10;
				a.needs.thirstVal += 10;
			}
		}
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(initVars->window))
		{
			return false;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int x = e.button.x;
			int y = e.button.y;

			SDL_Point mousePos = { x,y };

			//CLICKING AGENTS
			for (Agent a : agents)
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
			for (std::vector<Tile*>& v : grid->Tiles) //gets vectors of tiles
			{
				for (Tile*& t : v) // tiles in vector
				{
					if (WFCComponent->IsInTile(mousePos, *t)) //if tile is clicked
					{
						//if (start == nullptr) 
						//{
						//	start = t;
						//}
						//else if (end == nullptr && t != start)
						//{
						//	end = t;
						//}


						//if (start != nullptr && end != nullptr)
						//{
						//	AStar::ResetTiles(grid->Tiles);	
						//	AStar::Findpath(start, end);
						//	start = nullptr;
						//	end = nullptr;
						//}

						for (Agent& a : agents)
						{
							AStar::ResetTiles(grid->Tiles);
							a.GetState().from = a.position;
							a.GetState().to = glm::vec2(mousePos.x, mousePos.y);
							Tile* tile = a.GetTileFromPos(glm::vec2(a.position.x, a.position.y));
							a.GetState().path = AStar::Findpath(tile, t);

							if (a.GetState().path.size() > 1)
								a.GetState().path.erase(a.GetState().path.begin());				
						}
					}
				}
			}
		}
	}
	return true;
}

int main(int argc, char* argv[])
{
	InitVars returnVal = InitSDL();
	if (returnVal.initFail == true) return 0; //if init error
	InitVars* initVars = &returnVal;

	//TextureManager Init
	if (!TextureManager::TextureManagerInit(initVars->renderer, initVars->window)) return 0; //if error loading textures

	//ImGui Init
	ImGui_Implementation::Init(initVars->renderer, initVars->window);

	//Grid Init
	Grid* grid = new Grid(gridSizeX, gridSizeY, allTypes);

	//WFC Init
	WFC WFCComponent(grid);
	WFCComponent.WFCBody();
	grid->CreateRects(initVars->window);

	//temp - for A* test
	Tile* start = nullptr;
	Tile* end = nullptr;

	agents.reserve(10);

	//Agent init
	for (int i = 1; i < 11; i++)
	{
		ImGui_Implementation::agentCount = i;
		agents.emplace_back(grid, nullptr, nullptr);
	}
	
	Planner<MoveToState> plan{ &GoalComplete, {std::make_pair(MoveTo::Execute, MoveTo::IsValid)} };

	float accumulatedTime = 0;
	float counter = 0;

	//main loop
	while (true)
	{
		if (!PollEvents(grid, &WFCComponent, initVars)) break;

		//ImGui windows
		ImGui_Implementation::RenderBefore(); //setup
		ImGui_Implementation::AgentPopUp();
		ImGui_Implementation::MainUI(&WFCComponent);

		//RENDERING
		SDL_RenderClear(initVars->renderer); //remove anything already rendered

		WFCComponent.RenderWFC(initVars->renderer); //render WFC 

		if (!ImGui_Implementation::pause)
		{
			//DeltaTime
			static auto last = steady_clock::now();
			auto old = last;
			last = steady_clock::now();
			const duration<float> frameTime = last - old;
			float deltaTime = frameTime.count();

			accumulatedTime += deltaTime; //total accumulated
			counter += deltaTime; //counter for not executing every frame

			for (Agent& a : agents) //update agents
			{
				auto [executeFunc, completion] = plan.ActionSelector(a.GetState());

				switch (completion)
				{
				case InProgress:
					(*executeFunc)(a.GetState());
					break;
				case Complete:
					break;
				case Impossible:
					break;
				}

				a.Update(deltaTime);

				if (counter > 0.1 && ImGui_Implementation::agentCount == a.agentCount)
				{
					ImGui_Implementation::time.push_back(accumulatedTime);
					a.UpdateImGui();
					counter = 0;
				}
			}
		}

		for (Agent& a : agents) //render agents
		{
			a.Render(initVars->renderer, initVars->window);
		}

		ImGui_Implementation::ImGuiDraw(initVars->renderer); //render ImGUi
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
