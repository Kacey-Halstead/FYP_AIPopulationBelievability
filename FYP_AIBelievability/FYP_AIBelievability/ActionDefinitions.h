#pragma once
#include <vector>
#include <functional>

#include "Agent.h"

class DAG;

enum ActionIDs
{
	FOODACTION,
	FOODACTION2,
	WATERACTION,
	WATERACTION2,
	WANDER1
};

//Action - 2 functions (Execute and IsValid) 
using ExecuteFunc = std::function<void(States&)>;

using IsValidReturnType = bool; // TODO: change from bool to weighting to compare action desirability

using IsValidFunc = IsValidReturnType(*)(States&); //function pointer - smaller memory than using std::function

//action definition 
using Action = std::pair<std::pair<ExecuteFunc, IsValidFunc>, ActionIDs>;


namespace Actions
{
	enum ActionIndexes
	{
		FOOD,
		WATER,
		WANDER
	};

	extern std::vector<Action> foodActions;
	extern std::vector<Action> waterActions;
	extern std::vector<Action> wanderActions;

	extern std::vector<DAG> dags;

	DAG* GetDAG(ActionIndexes index);
	std::vector<Action> GetActions(ActionIndexes index);
}
