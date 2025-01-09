#pragma once
#include <vector>
#include <string>
#include "Action.h"

class GOAP
{
public:
	GOAP();
	~GOAP();

	void FormulateGoal();
	void FormulatePlan();
	void ExecutePlan();
	void StartGOAP();

private:
	std::vector<Action*> plan;
	std::pair<std::string, int> goal;
};

