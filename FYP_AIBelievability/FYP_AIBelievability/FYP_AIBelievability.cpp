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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std::chrono;

FYP_AIBelievability::FYP_AIBelievability() :
	mSDL{new SDLWindow()},
	mGrid{new Grid(allTypes)},
	mDAG{new DAG()},
	mFoodSources{ WFC::GetFoodSources() }
{
	FromJSONFile::ReadFromJSON();

	//TextureManager Init
	if (!TextureManager::TextureManagerInit(mSDL->getRenderer(), mSDL->getWindow())) throw std::runtime_error("Failed to load textures");

	//ImGui Init
	ImGui_Implementation::Init(mSDL->getRenderer(), mSDL->getWindow());

	//WFC init
	WFC::SetGridRef(mGrid.get());
	WFC::WFCReset();

	//Agent init
	mAgents.resize(50);
	for (int i = 1; i < 51; i++)
	{
		ImGui_Implementation::agentCount = index++;
		mAgents[i-1] = Agent(mGrid.get());
		mAgents[i - 1].Reset();

		if (i % 2 == 0)
		{
			mAgents[i-1].blueBushPref = true; // blue bush preference
		}
	}

	WFC::SetAgentsVec(&mAgents);

	index = 11;

	ImGui_Implementation::agentCount = mAgents[0].agentCount;
	ImGui_Implementation::OCEANValues = mAgents[0].personalityComponent.OCEANValues;
	ImGui_Implementation::Traits = mAgents[0].personalityComponent.traits;
	ImGui_Implementation::actions = mAgents[0].actions;
	ImGui_Implementation::emotionValues = mAgents[0].emotions;

	//Create all nodes for all actions
	auto Nodes = mDAG->CreateNodes(Actions::GetAllActions());

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

bool FYP_AIBelievability::MainLoop()
{
	if (!mSDL->Events(mGrid.get(), mAgents)) 
	{
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#endif // __EMSCRIPTEN__
		return false;
	}

	UpdateAgentsandFood();
	Update();
	Render();

	return true;
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

	for (int i = 0; i < foodSourcesIndex - 1; i++)
	{
		mFoodSources[i].Render(mSDL->getRenderer(), mSDL->getWindow());
		mFoodSources[i].isActive = true;
	}

	for (const Agent& a : mAgents) //render agents
	{
		if (a.agentCount == index) break;

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
		if (agent.agentCount >= index)
		{
			agent.active = false;
			continue;
		}
		agent.active = true;

		//if in water - go to nearest land tile
		Tile* currentTile = mGrid->GetTileFromPos(agent.position);
		if (currentTile && currentTile->GetType() == 'S')
		{			
			glm::vec2 toMove = mGrid->GridToWorldPos(mGrid->GetClosestTileOfType({ 'L', 'C' }, agent.position));
			agent.states.moveState.isMoveToSet = false;
			agent.Move(toMove);
		}
		else if (agent.states.moveState.isMoveToSet && !agent.states.socialState.isTalkingTo)
		{
			if (agent.states.moveState.to.x < 0 || agent.states.moveState.to.x > (gridSizeX - 1) ||
				agent.states.moveState.to.y < 0 || agent.states.moveState.to.y >(gridSizeY - 1)) agent.states.moveState.path.clear();

			if (!agent.states.moveState.path.empty())
			{

				glm::vec2 toGo = agent.states.moveState.path[0].tile->GetWorldPos();

				agent.Move(toGo);

				if (ComparePositions(agent.position, toGo, 1.0f))
				{
					agent.states.moveState.path.erase(agent.states.moveState.path.begin());
				}
			}
			else if(ComparePositions(agent.position, agent.states.moveState.to, 0.5f))
			{
				agent.states.moveState.isMoveToSet = false;
			}
			else
			{
				agent.states.agent->Move(agent.states.moveState.to);
			}
		}
		else if (!agent.responsiveStack.empty()) //if more responsive task comes up
		{
			DagNode* toExecute = mDAG->FindNode(agent.responsiveStack.top());
			Action* action = toExecute->action;

			if (action->isValidFunc(agent.states).second != InProgress)
			{
				agent.responsiveStack.pop();
			}

			action->executeFunc(agent.states);

			if (!agent.actions.empty() && agent.actions.front() != action->actionName)
			{
				agent.actions.push_front(action->actionName);
			}
		}
		else
		{
			Action goalAction = Goals::PickGoal(agent.states);
			DagNode* currentNode = mDAG->FindPlan(goalAction.ID, agent.states);

			if (currentNode != nullptr && !agent.states.socialState.isTalkingTo)
			{
				currentNode->action->executeFunc(agent.states);

				if (!agent.actions.empty() && agent.actions.front() != currentNode->action->actionName)
				{
					agent.actions.push_front(currentNode->action->actionName);
				}

				if (agent.actions.size() > 10)
				{
					agent.actions.pop_back();
				}
			}
		}

		agent.Update(deltaTime);

		if (ImGui_Implementation::agentCount > ImGui_Implementation::agentNumber)
		{
			ImGui_Implementation::agentCount = 1;
		}

		agent.UpdateImGui();

		//update ImGui
		if (mCounter > 0.1 && ImGui_Implementation::agentCount == agent.agentCount)
		{
			ImGui_Implementation::time.push_back(mAccumulatedTime);

			ImGui_Implementation::hungerValues = agent.GetValuesForImGui(0);
			ImGui_Implementation::thirstValues = agent.GetValuesForImGui(1);
			ImGui_Implementation::socialValues = agent.GetValuesForImGui(2);
			ImGui_Implementation::emotionValues = agent.emotions;
			ImGui_Implementation::actions = agent.actions;

			mCounter = 0;
		}

		//detect food sources
		for (FoodSource& foodSource : mFoodSources)
		{
			if (foodSource.isActive && foodSource.canEat && ComparePositions(agent.position, foodSource.position, 4.0f))
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

void FYP_AIBelievability::UpdateAgentsandFood()
{
	//spawn/delete agents or food sources

	if (ImGui_Implementation::agentNumber > (index - 1)) //more added
	{
		int toAdd = ImGui_Implementation::agentNumber - (index-1);
		for (int i = 0; i < toAdd - 1; i++)
		{
			ImGui_Implementation::agentCount = index++;
			mAgents[index - 1].Reset();
		}
	}
	else if (ImGui_Implementation::agentNumber < (index - 1))
	{
		index = ImGui_Implementation::agentNumber + 1;
	}

	if (ImGui_Implementation::foodNumber > (foodSourcesIndex - 1)) //more added
	{
		int toAdd = ImGui_Implementation::foodNumber - (foodSourcesIndex - 1);
		for (int i = 0; i < toAdd - 1; i++)
		{
			foodSourcesIndex++;
			mFoodSources[foodSourcesIndex - 1].isActive = true;
		}
	}
	else if (ImGui_Implementation::foodNumber < (foodSourcesIndex - 1)) //some taken away
	{
		foodSourcesIndex = ImGui_Implementation::foodNumber + 1;
		mFoodSources[foodSourcesIndex - 1].isActive = false;
	}
}
