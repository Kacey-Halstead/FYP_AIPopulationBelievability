#include "Action.h"
#include "DAG.h"

std::pair<IsGoalComplete, std::vector<Action>*> Goals::PickGoal(States& states)
{
	int index = states.moveState.agent->DecideOnGoal();

	switch (index)
	{
	case 0:

		return std::make_pair(FoodGoalComplete, Actions::GetActions(Actions::FOOD));
		break;
	case 1:
		return std::make_pair(ThirstGoalComplete, Actions::GetActions(Actions::WATER)); //CHANGE TO THIRST ACTIONS
		break;
	case 2:
		return std::make_pair(GoalComplete, Actions::GetActions(Actions::WANDER)); //CHANGE TO THIRST ACTIONS
		break;
	}

	return std::make_pair(FoodGoalComplete, Actions::GetActions(Actions::FOOD));
}
