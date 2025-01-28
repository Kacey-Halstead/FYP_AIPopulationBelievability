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
		position.x = 30 * ((float)rand() / RAND_MAX);
		position.y = 30* ((float)rand() / RAND_MAX);
	}
	else
	{
		//generates personality from parents
		personalityComponent = PersonalityComponent(P1, P2);
		parents[0] = P1;
		parents[1] = P2;
	}

	gridRef = grid;

	//sets extern variables
	ImGui_Implementation::OCEANValues = personalityComponent.OCEANValues;
	ImGui_Implementation::Traits = personalityComponent.traits;
	ImGui_Implementation::needStruct = needs;

	for (int i = 0; i < 5; i++)
	{
		states.findState.patrolPoints[i] = patrolPositions[i];
	}
}

Agent::~Agent()
{
	
}

void Agent::Update(float deltaTime)
{
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
	velocity = glm::vec2(0.0f);

	DecreaseNeeds(deltaTime);
}

void Agent::Render(SDL_Renderer* renderer, SDL_Window* window) const
{
	SDL_Rect destRect = gridRef->GetRenderRect(position, size);
	SDL_RenderCopy(renderer, TextureManager::GetTexture(AGENT), NULL, &destRect);
	SDL_RenderDrawRect(renderer, &destRect);
}

bool Agent::IsPointInAgent(SDL_Point point)
{
	return SDL_PointInRect(&point, &agentRect);
}

void Agent::DetectFood(std::pair<glm::vec2, FoodSource*> foodPair)
{
	auto it = std::find(states.foodState.prevFoodPositions.begin(), states.foodState.prevFoodPositions.end(), foodPair);
	if (it == states.foodState.prevFoodPositions.end())
	{
		states.foodState.prevFoodPositions.push_back(foodPair);
	}
}

void Agent::DetectWater(glm::vec2 pos)
{
	auto it = std::find(states.waterState.prevWaterPositions.begin(), states.waterState.prevWaterPositions.end(), pos);
	if (it == states.waterState.prevWaterPositions.end())
	{
		states.waterState.prevWaterPositions.push_back(pos);
	}
}

void Agent::DrinkWater(float amount)
{
	needs.thirstVal += amount;
	if (needs.thirstVal > 100.0f)
	{
		needs.thirstVal = 100.0f;
	}
}

int Agent::DecideOnGoal()
{
	float hungerUtility = sqrt((100 - needs.hungerVal) / 100);
	float thirstUtility = sqrt((100 - needs.thirstVal) / 100);

	if (hungerUtility > thirstUtility && needs.hungerVal < 40)
	{
		return 0; 
	}
	else if (thirstUtility >= hungerUtility && needs.thirstVal < 40 )
	{
		return 1;
	}
	else
	{
		return 2;
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
		needs.thirstVal -= 2 * deltaTime;
	}
}

void Agent::Move(glm::vec2 destination)
{
	glm::vec2 toDest = destination - position;
	if (toDest == glm::vec2(0.0f))
		return;
	velocity = glm::normalize(toDest) * speed;
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


