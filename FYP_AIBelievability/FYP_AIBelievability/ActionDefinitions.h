#pragma once
#include <vector>
#include <functional>
#include "Agent.h"


class DAG;


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
using Action = std::pair<std::pair<ExecuteFunc, IsValidFunc>, int>;

namespace Actions
{

	enum ActionIndexes
	{
		FOOD,
		WATER,
		WANDER,
		SOCIAL
	};

	extern std::vector<Action> foodActions;
	extern std::vector<Action> waterActions;
	extern std::vector<Action> wanderActions;

	std::vector<Action>* GetActions(ActionIndexes index);

	std::string Getname(int IDs);

	int FindID(std::string nameToSearch);

	int Counter(std::string nameOfAction);

	Action MakeAction(ExecuteFunc executeFunc, IsValidFunc isValidFunc, int ID);
}
