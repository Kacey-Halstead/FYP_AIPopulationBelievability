#include "Action.h"
#include "DAG.h"

Action Goals::PickGoal(States& states)
{
	std::pair<char, EEmotions> goal = states.moveState.agent->DecideOnGoal();

	switch (goal.first)
	{
	case 'H':
		return Actions::GetGoalAction(Actions::FOOD, goal.second);
	case 'T':
		return Actions::GetGoalAction(Actions::WATER, goal.second);
	case 'S':
		return Actions::GetGoalAction(Actions::SOCIAL, goal.second);
	case 'W':
		return Actions::GetGoalAction(Actions::WANDER, goal.second);
	}

	return Action();
}
