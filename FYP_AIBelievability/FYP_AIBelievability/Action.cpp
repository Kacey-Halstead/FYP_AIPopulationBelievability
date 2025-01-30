#include "Action.h"
#include "DAG.h"

std::vector<Action>* Goals::PickGoal(States& states)
{
	char index = states.moveState.agent->DecideOnGoal();

	switch (index)
	{
	case 'H':
		return Actions::GetActions(Actions::FOOD);
		break;
	case 'T':
		return Actions::GetActions(Actions::WATER); 
		break;
	case 'S':
		return Actions::GetActions(Actions::SOCIAL);
		break;
	case 'W':
		return Actions::GetActions(Actions::WANDER);
		break;
	}

	return nullptr;
}
