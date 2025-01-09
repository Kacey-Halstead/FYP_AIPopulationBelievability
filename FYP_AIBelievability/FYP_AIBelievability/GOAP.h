#pragma once
#include <vector>
#include <string>
#include "Action.h"

class Agent;

class GOAP
{
public:
	GOAP();
	~GOAP();

	void FormulateGoal(Agent* agent);
	void FormulatePlan(Agent* agent);
	void ExecutePlan();
	void StartGOAP(Agent* agent);

private:
	std::vector<Action*> plan;
	std::pair<std::string, int> goal;
};

