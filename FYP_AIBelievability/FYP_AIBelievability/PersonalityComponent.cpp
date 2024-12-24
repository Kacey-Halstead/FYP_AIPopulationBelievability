#include "PersonalityComponent.h"
#include "FromJSON.h"

PersonalityComponent::PersonalityComponent(Agent* parent1, Agent* parent2)
{
	GenerateFromParents(parent1, parent2);
}

PersonalityComponent::PersonalityComponent()
{
	RandomGeneration();
}

PersonalityComponent::~PersonalityComponent()
{
}

void PersonalityComponent::RandomGeneration()
{
	std::uniform_int_distribution<> distrib(1, FromJSONFile::traitsAndValues.size() - 1);
	int index = 0;
	for (int i = 0; i < 6; i++) //randomise 6 traits
	{
		index = distrib(RandomGenerator::gen);
		traits[i].traitName = FromJSONFile::traitsAndValues[index].traitName; //sets trait name
		traits[i].traitEffect = FromJSONFile::traitsAndValues[index].traitEffect; //sets trait effect (-1/1)
		traits[i].OCEANEffect = FromJSONFile::traitsAndValues[index].OCEANEffect; //sets effected OCEAN value
		OCEANValues[FromJSONFile::traitsAndValues[index].OCEANEffect] += FromJSONFile::traitsAndValues[index].traitEffect; //creates changed OCEAN values
	}
}

void PersonalityComponent::GenerateFromParents(Agent* parent1, Agent* parent2)
{

}
