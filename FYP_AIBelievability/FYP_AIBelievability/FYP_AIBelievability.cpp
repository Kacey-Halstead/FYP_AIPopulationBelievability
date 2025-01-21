#include <chrono>

#include "ImGuiImplementation.h"
#include "WFC.h"
#include "Agent.h"
#include "FromJSON.h"
#include "AStar.h"
#include "Action.h"
#include "SDLWindow.h"
#include "FoodSource.h"

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
	
	//Planner<MoveToState> plan{ &GoalComplete, {std::make_pair(MoveTo::Execute, MoveTo::IsValid)} };

	Planner<MoveToState, FindFoodState> foodPlan{ &FoodGoalComplete, {
		std::make_pair(FindFood::Execute, FindFood::IsValid),
		std::make_pair(MoveTo<FindFoodState>::Execute, MoveTo<FindFoodState>::IsValid),
		std::make_pair(EatFood::Execute, EatFood::IsValid)} };
	
	float accumulatedTime = 0;
	float counter = 0;

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
				auto [executeFunc, completion] = foodPlan.ActionSelector(a.GetState(), a.GetFoodState());

				switch (completion)
				{
				case InProgress:
					(*executeFunc)(a.GetState(), a.GetFoodState());
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

				for (FoodSource& f : food)
				{
					if (f.isInRect(a.position) && f.canEat)
					{
						a.GetFoodState().foundFoodRef = &f;
						a.DetectFood(true, f.position);

						//if (a.GetState().path.size() > 1)
						//	a.GetState().path.erase(a.GetState().path.begin());

					}
					//else
					//{
					//	a.GetFoodState().foundFoodRef = nullptr;
					//	a.DetectFood(false, {0,0});
					//}
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
