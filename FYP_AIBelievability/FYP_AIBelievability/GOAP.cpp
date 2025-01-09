#include "GOAP.h"

GOAP::GOAP()
{
}

GOAP::~GOAP()
{
}

void GOAP::FormulateGoal()
{

}

void GOAP::FormulatePlan()
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

void GOAP::StartGOAP()
{
	FormulateGoal();

	while (true) //goal not fulfilled 
	{
		FormulatePlan();

		ExecutePlan();
	}
}
