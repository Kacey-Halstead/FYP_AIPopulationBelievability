#pragma once
#include <vector>
#include <functional>
#include "Agent.h"
#include "Commons.h"

class DAG;

enum actionIDs : unsigned int
{
	GOAL_EATFOOD,
	FIND_FOOD,
	GOAL_DRINKWATER,
	FIND_WATER,
	GOAL_WANDER,
	GOAL_TRANSFERINFO,
	FIND_OTHER_AGENT,
	GOAL_FIGHT,
	FLEE
};

enum ActionProgress
{
	InProgress,
	Complete,
	Impossible
};

//Action - 2 functions (Execute and IsValid) 
using ExecuteFunc = std::function<void(States&)>;

using IsValidReturnType = std::pair<ActionProgress, int>; // TODO: change from bool to weighting to compare action desirability

using IsValidFunc = IsValidReturnType(*)(States&); //function pointer - smaller memory than using std::function

//action definition 

struct Action
{
	ExecuteFunc executeFunc;
	IsValidFunc isValidFunc;
	actionIDs ID;
	std::string actionName;
	EEmotions emotionNeeded;
};

//using Action = std::pair<std::pair<ExecuteFunc, IsValidFunc>, int>;

namespace Actions
{
	enum ActionIndexes
	{
		FOOD,
		WATER,
		WANDER,
		SOCIAL
	};

	std::vector<Action*> GetAllActions();

	Action GetAction(actionIDs ID);

	int FindID(std::string nameToSearch);

	int Counter(std::string nameOfAction);

	Action GetGoalAction(ActionIndexes actionIndex, EEmotions emotion);
}
