#include "Agent.h"
#include "RandomGenerator.h"

#include "JSON.h"


Agent::Agent()
{
	agentCount = ImGui_Implementation::agentCount;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { position.x, position.y, size.x, size.y };

	//if no parents, randomly generate traits
	if (parents[0] == nullptr)
	{
		GenerateTraits();
	}

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

void Agent::CalculateOCEANVals()
{

}

void Agent::GenerateTraits()
{
	std::ifstream file("traits.json");
	json j = json::parse(file);
	std::vector<std::string> traitNames;

	for (json& object : j) //OCEAN
	{
		for (auto& trait : object.items()) //traits
		{
			traitNames.emplace_back(trait.key());
		}
	}
	
	std::uniform_int_distribution<> distrib(1, 6);
	for (int i = 0; i < 6; i++)
	{
		OCEANVals[i] = distrib(RandomGenerator::gen);
	}

}
