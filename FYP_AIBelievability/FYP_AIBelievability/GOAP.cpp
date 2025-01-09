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

void GOAP::ExecutePlan()
{
	for (Action* a : plan)
	{

		//execute actions
		a->Execute();
	}
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
