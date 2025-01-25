#include <chrono>

#include "ImGuiImplementation.h"
#include "WFC.h"
#include "Agent.h"
#include "FromJSON.h"
#include "AStar.h"
#include "Action.h"
#include "SDLWindow.h"
#include "FoodSource.h"
#include "DAG.h"
#include "ActionDefinitions.h"

using namespace std::chrono;

std::vector<Agent> agents;

int main(int argc, char* argv[])
{
	SDLWindow SDL = SDLWindow();
	if (!SDL.InitSDL()) return 0;

	//unsigned int seed = time(nullptr);
	unsigned int seed = 'k' + 'a' + 'c' + 'e' + 'y';
	srand(seed);

	FromJSONFile::ReadFromJSON();

	//TextureManager Init
	if (!TextureManager::TextureManagerInit(SDL.getRenderer(), SDL.getWindow())) return 0; //if error loading textures

	//ImGui Init
	ImGui_Implementation::Init(SDL.getRenderer(), SDL.getWindow());

	//Grid Init
	Grid* grid = new Grid(gridSizeX, gridSizeY, allTypes);

	//WFC Init
	WFC::WFCBody(grid);
	grid->CreateRects(SDL.getWindow());

	//Agent init
	agents.reserve(10);
	for (int i = 1; i < 11; i++)
	{
		ImGui_Implementation::agentCount = i;
		agents.emplace_back(grid, nullptr, nullptr);
	}

	std::vector<FoodSource> food;
	//Food init
	for (int i = 0; i < 5; i++)
	{
		food.push_back(grid);
	}

	Actions::dags.push_back(DAG(Actions::foodActions));
	Actions::dags.push_back(DAG(Actions::waterActions));
	Actions::dags.push_back(DAG(Actions::wanderActions));

	Actions::GetDAG(Actions::FOOD)->AddRelation(&Actions::GetActions(Actions::FOOD)[0], &Actions::GetActions(Actions::FOOD)[1]);
	Actions::GetDAG(Actions::WATER)->AddRelation(&Actions::GetActions(Actions::WATER)[0], &Actions::GetActions(Actions::WATER)[1]);

	float accumulatedTime = 0;
	float counter = 0;

	Planner plan;

	//main loop
	while (true)
	{
		if (!SDL.Events(grid, agents)) break;

		//ImGui windows
		ImGui_Implementation::RenderBefore(); //setup
		ImGui_Implementation::AgentPopUp();
		ImGui_Implementation::MainUI();

		//RENDERING
		SDL.BeginRender();

		grid->RenderGrid(SDL.getRenderer()); //render WFC 

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

			//FOOD
			for (FoodSource f : food)
			{
				f.Update(deltaTime);
				f.Render(SDL.getRenderer(), SDL.getWindow());
			}

			for (Agent& a : agents) //update agents
			{

				//movement
				if (a.GetStates().moveState.isMoveToSet)
				{
					if (!a.GetStates().moveState.path.empty())
					{
						glm::vec2 toGo = a.GetStates().moveState.path[0].tile->GetWorldPos();

						a.GetStates().moveState.agent->Move(toGo);

						if (ComparePositions(a.GetStates().moveState.agent->position, toGo, 5))
						{
							a.GetStates().moveState.path.erase(a.GetStates().moveState.path.begin());
						}
					}

					if (ComparePositions(a.GetStates().moveState.agent->position, a.GetStates().moveState.to, 5) || a.GetStates().moveState.path.empty())
					{
						a.GetStates().moveState.isMoveToSet = false;
					}
				}

				//for every agent
				//decide on a priority (food, water, etc.)
				//cycle through actions and decide best path
				//once have plan, execute

				//GOAP selection


				
				auto [executeFunc, completion] = plan.ActionSelector(a.GetStates());



				switch (completion)
				{
				case InProgress:
				{
					(*executeFunc)(a.GetStates());
				}
					break;
				case Complete:
				{
					std::pair<std::pair<IsGoalComplete, std::vector<Action>>, DAG*> goalPair = Goals::PickGoal(a.GetStates());
					goalPair.second->ClearPlan();
					if (goalPair.second->FindPlan(&goalPair.first.second[0], a.GetStates()))
					{
						plan.SetPlan(goalPair.first.first, goalPair.second->GetAction());
					}
				}
					break;
				case Impossible:
				{
					std::pair<std::pair<IsGoalComplete, std::vector<Action>>, DAG*> goalPair = Goals::PickGoal(a.GetStates());
					goalPair.second->ClearPlan();
					if (goalPair.second->FindPlan(&goalPair.first.second[0], a.GetStates()))
					{
						plan.SetPlan(goalPair.first.first, goalPair.second->GetAction());
					}
				}
					break;
				}

				a.Update(deltaTime);

				//update ImGui
				if (counter > 0.1 && ImGui_Implementation::agentCount == a.agentCount)
				{
					ImGui_Implementation::time.push_back(accumulatedTime);
					a.UpdateImGui();
					counter = 0;
				}



				//detect food sources
				for (FoodSource& f : food)
				{
					if (f.isInRect(a.position) && f.taken && a.GetStates().foodState.foundFoodRef == nullptr)
					{
						a.GetStates().foodState.foundFoodRef = &f;
						a.DetectFood(true, f.position);
						f.taken = false;
					}

				}

				for (glm::vec2 pos : grid->waterPositions)
				{
					//detect water
					glm::vec2 worldPos = grid->GetPosFromTile(grid->Tiles[pos.x][pos.y]);

					if (ComparePositions(a.position, worldPos, 10) && !a.GetStates().waterState.waterRefSet)
					{
						a.GetStates().waterState.foundWaterRef = worldPos;
						a.GetStates().waterState.waterRefSet = true;
						a.DetectWater(true, worldPos);
					}
				}

			}
		}

		for (Agent& a : agents) //render agents
		{
			a.Render(SDL.getRenderer(), SDL.getWindow());
		}

		ImGui_Implementation::ImGuiDraw(SDL.getRenderer()); //render ImGUi
		SDL.EndRender();
	}

	// Cleanup
	ImGui_Implementation::Destroy();
	SDL.DestroySDL();
	return 0;
}
