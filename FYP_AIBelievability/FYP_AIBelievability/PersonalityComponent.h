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

	std::vector<int> OCEANValues = {0, 0, 0, 0, 0};
	std::vector<Trait> traits{};
	std::vector<float> emotionMultipliers = {1, 1, 1, 1, 1, 1, 1, 1};

private:
	void RandomGeneration();
	void GenerateFromParents(Agent* parent1, Agent* parent2);
	void SetMultipliers();


};

