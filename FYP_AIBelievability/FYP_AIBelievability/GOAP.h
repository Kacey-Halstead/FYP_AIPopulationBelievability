#pragma once
#include <vector>
#include <string>

class Agent;
class Action;

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
	//std::vector<BaseAction*> plan;
};

