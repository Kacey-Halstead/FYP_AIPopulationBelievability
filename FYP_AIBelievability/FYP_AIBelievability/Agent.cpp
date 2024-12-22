#include "Agent.h"
#include "RandomGenerator.h"

#include "JSON.h"


Agent::Agent()
{
	agentCount == ImGui_Implementation::agentCount;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { position.x, position.y, size.x, size.y };

	//if no parents, randomly generate traits
	if (parents[0] == nullptr)
	{
		GenerateTraits();
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

struct TraitsAndValues
{
	std::string traitName;
	int OCEANValue;
	int TraitEffect;

	TraitsAndValues(std::string name, int oceanVal, int traitVal)
	{
		traitName = name;
		OCEANValue = oceanVal;
		TraitEffect = traitVal;
	}
};

void Agent::GenerateTraits()
{
	std::ifstream file("traits.json");
	json j = json::parse(file);
	std::vector<TraitsAndValues> traitsAndValues; //trait name, OCEAN val, effect
	std::vector<std::string> traitNames;
	std::vector<int> traitValues;

	int currentType = 0;
	for (json& object : j) //OCEAN
	{
		for (auto& trait : object.items()) //traits
		{
			if (trait.key() == "TYPE")
			{
				currentType = trait.value(); // sets OCEAN type
			}
			else
			{
				TraitsAndValues tempTAV = { trait.key(), currentType, trait.value() }; //trait name, OCEAN val, trait val
				traitsAndValues.emplace_back(tempTAV);
			}
		}
	}
	
	std::uniform_int_distribution<> distrib(1, traitsAndValues.size()-1);
	int index = 0;
	for (int i = 0; i < 6; i++) //randomise 6 traits
	{
		index = distrib(RandomGenerator::gen);
		traits[i] = traitsAndValues[index].traitName;
		OCEANVals[traitsAndValues[index].OCEANValue] += traitsAndValues[index].TraitEffect;
	}

	ImGui_Implementation::OCEANValues = OCEANVals;
	ImGui_Implementation::Traits = traits;
}
