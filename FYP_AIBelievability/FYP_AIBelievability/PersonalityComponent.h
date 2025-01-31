#pragma once
#include "RandomGenerator.h"
#include <string>
#include <array>
#include"Commons.h"

class Agent;



class PersonalityComponent
{
public:
	PersonalityComponent(Agent* parent1, Agent* parent2); //generate personality from parents
	PersonalityComponent(); //assume random generation
	~PersonalityComponent();

	std::array<int, 5> OCEANValues{};
	std::array<Trait, 6> traits{};
	std::array<float, 8> emotionMultipliers{};

private:
	void RandomGeneration();
	void GenerateFromParents(Agent* parent1, Agent* parent2);
	void SetMultipliers();


};

