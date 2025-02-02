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
	for (int i = 1; i < 6; i++)
	{
		ImGui_Implementation::agentCount = i;
		mAgents.emplace_back(mGrid.get(), nullptr, nullptr);
	}

	//Create all nodes for all actions
	auto Nodes = mDAG->CreateNodes(Actions::GetAllActions());

	//Drink Water - dependencies: Find Water, Wander
	Nodes[GOAL_DRINKWATER]->children.push_back(Nodes[FIND_WATER]);
	Nodes[GOAL_DRINKWATER]->children.push_back(Nodes[GOAL_WANDER]);

	//Eat Food - dependencies: Find Food, Wander
	Nodes[GOAL_EATFOOD]->children.push_back(Nodes[FIND_FOOD]);
	Nodes[GOAL_EATFOOD]->children.push_back(Nodes[GOAL_WANDER]);

	//Transfer knowledge - dependencies: FindOtherAgent
	Nodes[GOAL_TRANSFERINFO]->children.push_back(Nodes[FIND_OTHER_AGENT]); //trust

	//Fight - dependencies: FindOtherAgent
	Nodes[GOAL_FIGHT]->children.push_back(Nodes[FIND_OTHER_AGENT]); //anger

	//Socialise - dependencies: FindOtherAgent
	Nodes[GOAL_SOCIALISE]->children.push_back(Nodes[FIND_OTHER_AGENT]); //joy
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

	deltaTime *= ImGui_Implementation::deltaTimeModifier;
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
		if (agent.states.moveState.isMoveToSet && !agent.states.socialState.isTalkingTo)
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
		else if (!agent.responsiveStack.empty()) //is more responsive task comes up
		{
			DagNode* toExecute = mDAG->FindNode(agent.responsiveStack.top());
			Action* action = toExecute->action;
			action->executeFunc(agent.states);

			if (action->isValidFunc(agent.states).first != InProgress)
			{
				agent.responsiveStack.pop();
			}
		}
		else
		{
			Action goalAction = Goals::PickGoal(agent.states);
			DagNode* currentNode = mDAG->FindPlan(goalAction.ID, agent.states);

			if (currentNode != nullptr && !agent.states.socialState.isTalkingTo)
			{
				currentNode->action->executeFunc(agent.states);
				agent.actions.push_front(currentNode->action->actionName);

				if (agent.actions.size() > 10)
				{
					agent.actions.pop_back();
				}
			}
		}

		agent.Update(deltaTime);


		//update ImGui
		if (mCounter > 0.1 && ImGui_Implementation::agentCount == agent.agentCount)
		{
			ImGui_Implementation::time.push_back(mAccumulatedTime);
			agent.UpdateImGui();

			if (ImGui_Implementation::isAgentPressed)
			{
				ImGui_Implementation::hungerValues = agent.GetValuesForImGui(0);
				ImGui_Implementation::thirstValues = agent.GetValuesForImGui(1);
				ImGui_Implementation::socialValues = agent.GetValuesForImGui(2);
				ImGui_Implementation::emotionValues = agent.emotions;
				ImGui_Implementation::actions = agent.actions;
			}

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

		//detect water
		for (glm::ivec2 pos : mGrid->waterPositions)
		{
			if (ComparePositions(agent.position, pos, 4.0f))
			{
				agent.DetectWater(pos);
			}
		}

		//detect other agents
		for (Agent& otherAgent : mAgents)
		{
			if (otherAgent.agentCount == agent.agentCount) continue;

			//if close enough to other agent, add
			if (ComparePositions(agent.position, otherAgent.position, 4.0f))
			{
				agent.DetectOtherAgents(&otherAgent);
			}
		}
	}
}