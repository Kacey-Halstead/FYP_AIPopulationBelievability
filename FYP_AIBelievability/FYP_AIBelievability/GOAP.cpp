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
	MoveTo* move{};
	agent->m.to = agent->GetTileFromPos({10, 10});
	agent->m.from = agent->GetTileFromPos(SDL_Point(agent->position.x, agent->position.y));
	agent->m.agent = agent;

	if (move->IsValid(&agent->m))
	{
		move->Execute(&agent->m);		
	}

}

void GOAP::StartGOAP(Agent* agent)
{
	FormulateGoal(agent);

	FormulatePlan(agent);

	ExecutePlan(agent);
}
