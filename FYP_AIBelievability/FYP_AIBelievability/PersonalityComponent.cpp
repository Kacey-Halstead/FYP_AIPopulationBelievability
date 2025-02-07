#include "PersonalityComponent.h"
#include "FromJSON.h"

PersonalityComponent::PersonalityComponent(Agent* parent1, Agent* parent2)
{
	GenerateFromParents(parent1, parent2);

}

PersonalityComponent::PersonalityComponent()
{
	RandomGeneration();
	SetMultipliers();
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
		Trait trait{};
		trait.traitName = FromJSONFile::traitsAndValues[index].traitName; //sets trait name
		trait.traitEffect = FromJSONFile::traitsAndValues[index].traitEffect; //sets trait effect (-1/1)
		trait.OCEANEffect = FromJSONFile::traitsAndValues[index].OCEANEffect; //sets affected OCEAN value
		traits.emplace_back(trait);
		OCEANValues[FromJSONFile::traitsAndValues[index].OCEANEffect] += FromJSONFile::traitsAndValues[index].traitEffect; //creates changed OCEAN values
	}
}

void PersonalityComponent::GenerateFromParents(Agent* parent1, Agent* parent2)
{

}

void PersonalityComponent::SetMultipliers()
{
	//for every trait, check ocean value to see affected emotions, then apply trait effect

	for (Trait& trait : traits)
	{
		for (int i = 0; i < 8; i++)
		{
			//emotion multiplier = +/- trait * whether high or low score trait. High score = normal, low score = converted values
			emotionMultipliers[i] += (OCEANMultipliers[trait.OCEANEffect][i] * trait.traitEffect);
		}
	}
}

/*
	Surprise
	Anticipation
	Disgust
	Joy
	Anger
	Fear
	Trust
	Sadness
*/