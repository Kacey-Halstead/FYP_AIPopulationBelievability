#include "GOAP.h"

GOAP::GOAP()
{
}

GOAP::~GOAP()
{
}

void GOAP::FormulateGoal(Agent* agent)
{
	//evaluate agent needs

}

void GOAP::FormulatePlan(Agent* agent)
{

}

void GOAP::ExecutePlan(Agent* agent)
{
	MoveTo* action = new MoveTo();
	std::vector<Action<ToMoveState>*> actions = {action};

	plan.ExecutePlan(actions);
}

void GOAP::StartGOAP(Agent* agent)
{
	FormulateGoal(agent);

	while (true) //goal not fulfilled 
	{
		FormulatePlan(agent);

		ExecutePlan();
	}
}
