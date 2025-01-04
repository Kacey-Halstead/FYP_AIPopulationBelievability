#pragma once
#include <vector>

#include "Action.h"

class GOAP
{
public:
	GOAP();
	~GOAP();

	void FormulateGoal();
	void FormulatePlan();
	void ExecutePlan();

private:
	std::vector<Action*> plan;

};

