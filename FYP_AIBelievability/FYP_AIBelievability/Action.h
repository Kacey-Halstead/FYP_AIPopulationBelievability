#pragma once
#include <vector>
#include <SDL.h>
#include "Tile.h"
#include "AStar.h"
#include "Commons.h"
#include "Agent.h"
#include "States.h"
#include "RandomGenerator.h"

#include <functional>

enum ActionProgress
{
	InProgress,
	Complete,
	Impossible
};

//Action - 2 functions (Execute and IsValid) 
template<typename... Structs>
using ExecuteFunc = std::function<void(Structs&...)>;

using IsValidReturnType = bool; // TODO: change from bool to weighting to compare action desirability

template<typename... Structs>
using IsValidFunc = IsValidReturnType(*)(Structs&...); //function pointer - smaller memory than using std::function

//action definition 
template<typename... Structs>
using Action = std::pair<ExecuteFunc<Structs...>, IsValidFunc<Structs...>>;


//Goal Completion function (IsGoalComplete)
template<typename... Structs>
using IsGoalComplete = std::function<bool(Structs&...)>;


//PLANNER
template<typename... Structs>
class Planner 
{
	using Action = Action<Structs...>;

public:
	Planner(IsGoalComplete<Structs...> goal, std::vector<Action>&& allActions) //planner requires goal complete func and actions in plan
	{
		isGoalComplete = goal;
		actions = allActions;
	}

	std::pair<const ExecuteFunc<Structs...>*, ActionProgress> ActionSelector(Structs&... states)
	{
		if (!isGoalComplete(states...))
		{
			//cycle through actions and decide action
			for (const auto& [executeFunc, isValidFunc] : actions)
			{
				if (isValidFunc(states...))
				{
					return std::make_pair(&executeFunc, InProgress);
				}
			}

			//no actions available and not complete
			return std::pair(nullptr, Impossible);
		}

		return std::pair(nullptr, Complete);
	}

private:
	IsGoalComplete<Structs...> isGoalComplete;
	std::vector<Action> actions;
};

//Defined Actions
struct MoveTo
{
	static void Execute(MoveToState& conditions)
	{
		if (!conditions.path.empty())
		{
			glm::vec2 toGo = conditions.path[0].tile->GetWorldPos();

			conditions.agent->Move(toGo);

			if (ComparePositions(conditions.agent->position, toGo))
			{
				conditions.path.erase(conditions.path.begin());
			}
		}
	}

	static bool IsValid(MoveToState& conditions)
	{
		return !ComparePositions(conditions.agent->position, conditions.to);
	}
};

struct FindFood
{
	static void Execute(MoveToState& conditions, FindFoodState& foodConditions)
	{
		//can check previous positions?
		if (!foodConditions.prevFoodPositions.empty())
		{
			foodConditions.nextToCheck = foodConditions.prevFoodPositions[0];
			conditions.to = foodConditions.nextToCheck;
			return;
		}

		std::uniform_int_distribution<> distrib(1, gridSizeX - 1);
		int triedPosX = distrib(RandomGenerator::gen);
		int triedPosY = distrib(RandomGenerator::gen);
		conditions.to.x = triedPosX * conditions.agent->size.x;
		conditions.to.y = triedPosY * conditions.agent->size.y;
	}

	static bool IsValid(MoveToState& conditions, FindFoodState& foodConditions)
	{
		return !foodConditions.isFoodFound;
	}
};

//Goal Completion Functions
static bool GoalComplete(MoveToState& state)
{
	if (ComparePositions(state.agent->position, state.to))
	{
		return true;
	}
	return false;
}