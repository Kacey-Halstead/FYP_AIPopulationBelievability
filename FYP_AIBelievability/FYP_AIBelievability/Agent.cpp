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
		position.x = floor(rand() % gridSizeX + 1);
		position.y = floor(rand() % gridSizeY + 1);

		agentRect = { (int)(position.x * gridRef->tileSize.x), (int)(position.y * gridRef->tileSize.y), size.x, size.y };

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
}

Agent::~Agent()
{
	
}

void Agent::Update(float deltaTime)
{
	agentRect.x = (position.x * gridRef->tileSize.x) - agentRect.w/2;
	agentRect.y = (position.y * gridRef->tileSize.y) - agentRect.h / 2;

	position.x += floor(velocity.x * deltaTime);
	position.y += floor(velocity.y * deltaTime);
	velocity = { 0.0f, 0.0f };

	DecreaseNeeds(deltaTime);
}

void Agent::Render(SDL_Renderer* renderer, SDL_Window* window) const
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture(AGENT), NULL, &agentRect);
	SDL_RenderDrawRect(renderer, &agentRect);
}

bool Agent::IsPointInAgent(SDL_Point point)
{
	return SDL_PointInRect(&point, &agentRect);
}

glm::vec2 Agent::ScreenPos()
{
	glm::vec2 screenPos = glm::vec2(position.x * gridSizeX, position.y * gridSizeY);

	return screenPos;
}

void Agent::DetectFood(glm::ivec2 pos)
{
	auto it = std::find(states.foodState.prevFoodPositions.begin(), states.foodState.prevFoodPositions.end(), pos);
	if (it == states.foodState.prevFoodPositions.end())
	{
		states.foodState.prevFoodPositions.push_back(pos);
	}
}

void Agent::DetectWater(glm::ivec2 pos)
{
	auto it = std::find(states.waterState.prevWaterPositions.begin(), states.waterState.prevWaterPositions.end(), pos);
	if (it == states.waterState.prevWaterPositions.end())
	{
		states.waterState.prevWaterPositions.push_back(pos);
	}
}

void Agent::DrinkWater(float amount)
{
	if (needs.thirstVal < (100 - amount))
	{
		needs.thirstVal += amount;
	}
}

int Agent::DecideOnGoal()
{
	float hungerUtility = sqrt((100 - needs.hungerVal) / 100);
	float thirstUtility = sqrt((100 - needs.thirstVal) / 100);

	if (hungerUtility > thirstUtility && hungerUtility < 40)
	{
		return 0; 
	}
	else if (thirstUtility <= hungerUtility && thirstUtility < 40 )
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

void Agent::Move(glm::ivec2 destination)
{
	glm::ivec2 toDest = destination - position;
	if (toDest == glm::ivec2(0, 0))
		return;
	velocity = toDest * speed;
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


