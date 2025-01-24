#include "Agent.h"

Agent::Agent()
{

}

Agent::Agent(Grid* grid, Agent* P1, Agent* P2)
{
	agentCount = ImGui_Implementation::agentCount;
	states.moveState.agent = this;

	if (P1 == nullptr) //if no parents
	{
		//generates random personality
		personalityComponent = PersonalityComponent();

		//random positions
		position.x = rand() % 200 + 1;
		position.y = rand() % 200 + 1;

		agentRect = { (int)position.x, (int)position.y, size.x, size.y };
		detectRect = { agentRect.x - ((agentRect.w * 5) / 2), agentRect.y - ((agentRect.h * 5) / 2), agentRect.w * 5, agentRect.h * 5 };

		if (grid != nullptr)
		{
			gridRef = grid;
		}
	}
	else
	{
		gridRef = P1->gridRef;

		agentRect = { (int)P1->position.x, (int)P1->position.y, size.x, size.y };

		//generates personality from parents
		personalityComponent = PersonalityComponent(P1, P2);
		parents[0] = P1;
		parents[1] = P2;
	}

	//sets extern variables
	ImGui_Implementation::OCEANValues = personalityComponent.OCEANValues;
	ImGui_Implementation::Traits = personalityComponent.traits;
	ImGui_Implementation::needStruct = needs;

	for (int i = 0; i < states.findState.patrolPoints.size(); i++)
	{
		states.findState.patrolPoints[i].first = patrolPositions[i] * gridRef->tileSize;
		states.findState.patrolPoints[i].second = false;
	}
}

Agent::~Agent()
{
	
}

void Agent::Update(float deltaTime)
{

	agentRect.x = position.x - agentRect.w/2;
	agentRect.y = position.y - agentRect.h/2;
	detectRect.x = position.x - detectRect.w / 2;
	detectRect.y = position.y - detectRect.h / 2;

	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
	velocity = { 0.0f, 0.0f };

	DecreaseNeeds(deltaTime);
}

void Agent::Render(SDL_Renderer* renderer, SDL_Window* window)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture(AGENT), NULL, &agentRect);
	SDL_RenderDrawRect(renderer, &agentRect);
	//SDL_RenderDrawRect(renderer, &detectRect);
}

bool Agent::IsPointInAgent(SDL_Point point)
{
	return SDL_PointInRect(&point, &agentRect);
}

void Agent::DetectFood(bool detect, glm::vec2 pos)
{
	states.findState.isFound = detect;
	states.moveState.isMoveToSet = detect;

	if (detect)
	{		
		states.moveState.to = pos;
		states.moveState.from = position;
		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);
	}

	auto it = std::find(states.foodState.prevFoodPositions.begin(), states.foodState.prevFoodPositions.end(), pos);
	if (it == states.foodState.prevFoodPositions.end())
	{
		states.foodState.prevFoodPositions.push_back(pos);
	}
}

int Agent::DecideOnGoal()
{
	float hungerUtility = sqrt((100 - needs.hungerVal) / 100);
	float thirstUtility = sqrt((100 - needs.thirstVal) / 100);

	if (hungerUtility > thirstUtility)
	{
		return 0; 
	}
	else
	{
		return 1;
	}
}

void Agent::DecreaseNeeds(float deltaTime)
{
	if (needs.hungerVal > 0)
	{
		needs.hungerVal -= 2 * deltaTime;

		if (needs.hungerVal < 80)
		{
			states.foodState.complete = false;
		}
		else
		{
			states.foodState.complete = true;
		}
	}

	if (needs.thirstVal > 0)
	{
		needs.thirstVal -= 0.2 * deltaTime;
	}
}

void Agent::Move(glm::vec2 destination)
{
	velocity = glm::normalize(destination - position) * speed;
}

void Agent::UpdateImGui()
{
	if (ImGui_Implementation::time.size() >= 400)
	{
		ImGui_Implementation::time.erase(ImGui_Implementation::time.begin());
	}

	if (ImGui_Implementation::hungerValues.size() >= 400)
	{
		ImGui_Implementation::hungerValues.erase(ImGui_Implementation::hungerValues.begin());
		ImGui_Implementation::thirstValues.erase(ImGui_Implementation::thirstValues.begin());
	}

	ImGui_Implementation::hungerValues.push_back(needs.hungerVal);
	ImGui_Implementation::thirstValues.push_back(needs.thirstVal);
}


