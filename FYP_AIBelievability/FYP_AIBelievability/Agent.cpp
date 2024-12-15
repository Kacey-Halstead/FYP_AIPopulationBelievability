#include "Agent.h"

Agent::Agent()
{
	agentCount = ImGui_Implementation::agentCount;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { position.x, position.y, size.x, size.y };

	for (int i = 0; i < 5; i++)
	{
		OCEANVals[i] = rand() % 5 + 1;
	}
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
