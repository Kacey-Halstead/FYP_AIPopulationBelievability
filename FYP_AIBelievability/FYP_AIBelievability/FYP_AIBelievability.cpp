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
	for (int i = 1; i < 11; i++)
	{
		ImGui_Implementation::agentCount = i;
		mAgents.emplace_back(mGrid.get(), nullptr, nullptr);
	}

	mDAG->CreateNode(Actions::GetActions(Actions::WATER));
	mDAG->CreateNode(Actions::GetActions(Actions::FOOD));
	mDAG->CreateNode(Actions::GetActions(Actions::WANDER));

	const std::vector<Action>* waterVec = Actions::GetActions(Actions::WATER);

	node* waterNode = mDAG->FindNode(waterVec->at(0).second);
	waterNode->children.push_back(mDAG->FindNode(waterVec->at(1).second));

	const std::vector<Action>* foodVec = Actions::GetActions(Actions::FOOD);

	node* foodNode = mDAG->FindNode(foodVec->at(0).second);
	foodNode->children.push_back(mDAG->FindNode(foodVec->at(1).second));
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
		if (agent.GetStates().moveState.isMoveToSet)
		{
			if (!agent.GetStates().moveState.path.empty())
			{
				glm::vec2 toGo = agent.GetStates().moveState.path[0].tile->GetWorldPos();

				agent.GetStates().moveState.agent->Move(toGo);

				if (ComparePositions(agent.position, toGo, 30))
				{
					agent.GetStates().moveState.path.erase(agent.GetStates().moveState.path.begin());
				}
			}

			if (ComparePositions(agent.GetStates().moveState.agent->position, agent.GetStates().moveState.to, 30) || agent.GetStates().moveState.path.empty())
			{
				agent.GetStates().moveState.isMoveToSet = false;
			}
		}
		else
		{
			auto [GoalComplete, actions] = Goals::PickGoal(agent.GetStates());
			node* currentNode = mDAG->FindPlan(actions->front().second, agent.GetStates());

			if (currentNode != nullptr)
			{
				currentNode->action->first.first(agent.GetStates());
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
			if (foodSource.IsInRect(agent.position))
			{
				agent.DetectFood(foodSource.position);

				if (ComparePositions(agent.position, foodSource.position, 20.0f))
				{
					agent.GetStates().foodState.foundFoodRef = &foodSource;
				}
			}
		}

		for (glm::ivec2 pos : mGrid->waterPositions)
		{
			//detect water
			glm::vec2 worldPos = mGrid->GridToWorldPos(pos);

			if (ComparePositions(agent.position, worldPos, 3 * mGrid->tileSize.x))
			{
				agent.DetectWater(worldPos);
			}
		}

	}
}