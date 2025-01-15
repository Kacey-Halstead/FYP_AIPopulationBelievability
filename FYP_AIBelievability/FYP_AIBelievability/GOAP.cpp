#include "GOAP.h"
#include "Agent.h"
#include "Action.h"


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

}

void GOAP::StartGOAP(Agent* agent)
{
	FormulateGoal(agent);

	FormulatePlan(agent);

	ExecutePlan(agent);
}
