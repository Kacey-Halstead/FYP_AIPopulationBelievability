#include "Agent.h"

Agent::Agent()
{
	agentCount = ImGui_Implementation::agentCount;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { position.x, position.y, size.x, size.y };

	//generates random personality
	personality = PersonalityComponent();

	//sets extern variables
	ImGui_Implementation::OCEANValues = personality.OCEANValues;
	ImGui_Implementation::Traits = personality.traits;
}

Agent::Agent(Agent* P1, Agent* P2)
{
	agentCount = ImGui_Implementation::agentCount;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { position.x, position.y, size.x, size.y };

	//generates personality from parents
	personality = PersonalityComponent(P1, P2);
	parents[0] = P1;
	parents[0] = P2;

	//sets extern variables
	ImGui_Implementation::OCEANValues = personality.OCEANValues;
	ImGui_Implementation::Traits = personality.traits;
}

Agent::~Agent()
{
	
}

void Agent::Update(float deltaTime)
{
	agentRect.x = position.x;
	agentRect.y = position.y;
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



