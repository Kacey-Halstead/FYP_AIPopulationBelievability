#pragma once
#include <vector>
#include <functional>

#include "Agent.h"

enum ActionIDs
{
	FOODACTION,
	FOODACTION2
};

//Action - 2 functions (Execute and IsValid) 
using ExecuteFunc = std::function<void(States&)>;

using IsValidReturnType = bool; // TODO: change from bool to weighting to compare action desirability

using IsValidFunc = IsValidReturnType(*)(States&); //function pointer - smaller memory than using std::function

//action definition 
using Action = std::pair<std::pair<ExecuteFunc, IsValidFunc>, ActionIDs>;

namespace Actions
{
	extern std::vector<Action> foodActions;

	std::vector<Action> GetActions(int index);
}
