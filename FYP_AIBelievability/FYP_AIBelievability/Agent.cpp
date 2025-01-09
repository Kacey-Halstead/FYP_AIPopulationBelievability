#include "Agent.h"

Agent::Agent()
{
	agentCount = ImGui_Implementation::agentCount;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { position.x, position.y, size.x, size.y };

	//generates random personality
	personalityComponent = PersonalityComponent();

	//Sets GOAP Component
	GOAPComponent = GOAP();

	//sets extern variables
	ImGui_Implementation::OCEANValues = personalityComponent.OCEANValues;
	ImGui_Implementation::Traits = personalityComponent.traits;
	ImGui_Implementation::needStruct = needs;

	GOAPComponent.ExecutePlan();
}

Agent::Agent(Agent* P1, Agent* P2)
{
	agentCount = ImGui_Implementation::agentCount;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { position.x, position.y, size.x, size.y };

	//generates personality from parents
	personalityComponent = PersonalityComponent(P1, P2);
	parents[0] = P1;
	parents[0] = P2;

	//Sets GOAP Component
	GOAPComponent = GOAP();

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
	agentRect.x = position.x;
	agentRect.y = position.y;

	position.x += velocity.x * deltaTime;
	position.x += velocity.y * deltaTime;

	DecreaseNeeds(deltaTime);
}

void Agent::Render(SDL_Renderer* renderer, SDL_Window* window)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture(AGENT), NULL, &agentRect);
	SDL_RenderDrawRect(renderer, &agentRect);
}

bool Agent::IsPointInAgent(SDL_Point point)
{
	return SDL_PointInRect(&point, &agentRect);
}

void Agent::DecreaseNeeds(float deltaTime)
{
	if (needs.hungerVal > 0)
	{
		needs.hungerVal -= 1 * deltaTime;
	}

	if (needs.thirstVal > 0)
	{
		needs.thirstVal -= 5 * deltaTime;
	}
}

bool Agent::Move(SDL_Point destination)
{
	while (destination != position)
	{
		velocity = destination - position;
	}

	return true;
}



