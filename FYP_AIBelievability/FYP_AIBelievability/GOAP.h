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
	void ExecutePlan(Agent* agent);
	void StartGOAP(Agent* agent);

private:
	Planner<ToMoveState> plan;
	std::pair<std::string, int> goal;
};

