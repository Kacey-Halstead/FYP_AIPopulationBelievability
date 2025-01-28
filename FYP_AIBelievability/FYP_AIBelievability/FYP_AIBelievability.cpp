#include "FYP_AIBelievability.h"
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

FYP_AIBelievability::FYP_AIBelievability() :
	mSDL{new SDLWindow()},
	mGrid{new Grid(allTypes)},
	mDAG{new DAG()}
{
	//unsigned int seed = time(nullptr);
	unsigned int seed = 'k' + 'a' + 'c' + 'e' + 'y';
	srand(seed);
	
	FromJSONFile::ReadFromJSON();

	//TextureManager Init
	if (!TextureManager::TextureManagerInit(mSDL->getRenderer(), mSDL->getWindow())) throw std::runtime_error("Failed to load textures");

	//ImGui Init
	ImGui_Implementation::Init(mSDL->getRenderer(), mSDL->getWindow());

	//WFC Init
	WFC::WFCBody(mGrid.get());
	for (int i = 0; i < 5; ++i)
	{
		mFoodSources.emplace_back(mGrid.get());
	}
	//Agent init
	mAgents.reserve(10);
	for (int i = 1; i < 2; i++)
	{
		ImGui_Implementation::agentCount = i;
		mAgents.emplace_back(mGrid.get(), nullptr, nullptr);
	}

	mDAG->CreateNode(Actions::GetActions(Actions::WATER));
	mDAG->CreateNode(Actions::GetActions(Actions::FOOD));
	mDAG->CreateNode(Actions::GetActions(Actions::WANDER));

	const std::vector<Action>* wanderVec = Actions::GetActions(Actions::WANDER);
	const std::vector<Action>* waterVec = Actions::GetActions(Actions::WATER);

	node* waterNode = mDAG->FindNode(waterVec->at(0).second);
	waterNode->children.push_back(mDAG->FindNode(waterVec->at(1).second));
	waterNode->children.push_back(mDAG->FindNode(wanderVec->at(0).second));

	const std::vector<Action>* foodVec = Actions::GetActions(Actions::FOOD);

	node* foodNode = mDAG->FindNode(foodVec->at(0).second);
	foodNode->children.push_back(mDAG->FindNode(foodVec->at(1).second));
	foodNode->children.push_back(mDAG->FindNode(wanderVec->at(0).second));

}

FYP_AIBelievability::~FYP_AIBelievability()
{
	// Cleanup
	ImGui_Implementation::Destroy();
}

void FYP_AIBelievability::MainLoop()
{
	//main loop
	while (true)
	{
		if (!mSDL->Events(mGrid.get(), mAgents)) break;

		Update();
		Render();
	}
}

void FYP_AIBelievability::Render() const
{
	//ImGui windows
	ImGui_Implementation::RenderBefore(); //setup
	ImGui_Implementation::AgentPopUp();
	ImGui_Implementation::MainUI();

	//RENDERING
	mSDL->BeginRender();

	mGrid->RenderGrid(mSDL->getRenderer()); //render WFC 

	for (const auto& foodSource : mFoodSources)
	{
		foodSource.Render(mSDL->getRenderer(), mSDL->getWindow());
	}

	for (const Agent& a : mAgents) //render agents
	{
		a.Render(mSDL->getRenderer(), mSDL->getWindow());
	}

	ImGui_Implementation::ImGuiDraw(mSDL->getRenderer()); //render ImGUi
	mSDL->EndRender();
}

void FYP_AIBelievability::Update()
{
	if (ImGui_Implementation::pause) return;

	//DeltaTime
	static auto last = steady_clock::now();
	auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	float deltaTime = frameTime.count();

	// large deltatime causes agents to move too far
	if (deltaTime > 1.0f)
		deltaTime = 0.0f;

	deltaTime *= 3;
	mAccumulatedTime += deltaTime; //total accumulated
	mCounter += deltaTime; //counter for not executing every frame

	//FOOD
	for (FoodSource& foodSource : mFoodSources)
	{
		foodSource.Update(deltaTime);
	}

	for (Agent& agent : mAgents) //update agents
	{
		//movement
		if (agent.states.moveState.isMoveToSet)
		{
			if (!agent.states.moveState.path.empty())
			{
				glm::vec2 toGo = agent.states.moveState.path[0].tile->GetWorldPos();

				agent.states.moveState.agent->Move(toGo);

				if (ComparePositions(agent.position, toGo, 1.0f))
				{
					agent.states.moveState.path.erase(agent.states.moveState.path.begin());
				}
			}
			else
			{
				if (ComparePositions(agent.position, agent.states.moveState.to, 0.5f))
				{
					agent.states.moveState.isMoveToSet = false;
				}
				else
				{
					agent.states.moveState.agent->Move(agent.states.moveState.to);
				}
			}
		}
		else
		{
			auto [GoalComplete, actions] = Goals::PickGoal(agent.states);
			node* currentNode = mDAG->FindPlan(actions->front().second, agent.states);

			if (currentNode != nullptr)
			{
				currentNode->action->first.first(agent.states);
				ImGui_Implementation::action = Actions::Getname(currentNode->action->second);
			}
		}

		agent.Update(deltaTime);

		//update ImGui
		if (mCounter > 0.1 && ImGui_Implementation::agentCount == agent.agentCount)
		{
			ImGui_Implementation::time.push_back(mAccumulatedTime);
			agent.UpdateImGui();
			mCounter = 0;
		}

		//detect food sources
		for (FoodSource& foodSource : mFoodSources)
		{
			if (foodSource.canEat && ComparePositions(agent.position, foodSource.position, 4.0f))
			{
				agent.DetectFood(make_pair(foodSource.position, &foodSource));
			}
		}

		for (glm::ivec2 pos : mGrid->waterPositions)
		{
			//detect water

			if (ComparePositions(agent.position, pos, 4.0f))
			{
				agent.DetectWater(pos);
			}
		}

	}
}