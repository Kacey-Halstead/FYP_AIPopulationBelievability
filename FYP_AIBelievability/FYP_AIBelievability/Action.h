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

template<typename... Structs> //so moveTo can be used in many plans
struct MoveTo
{
	static void Execute(MoveToState& conditions, Structs& ...)
	{
		conditions.from = conditions.agent->position;

		if (!conditions.path.empty())
		{
			glm::vec2 toGo = conditions.path[0].tile->GetWorldPos();

			conditions.agent->Move(toGo);

			if (ComparePositions(conditions.agent->position, toGo))
			{
				conditions.path.erase(conditions.path.begin());
			}
		}

		if (ComparePositions(conditions.agent->position, conditions.to) || conditions.path.empty())
		{
			conditions.isMoveToSet = false;
		}
	}

	static bool IsValid(MoveToState& conditions, Structs& ...)
	{
		return !ComparePositions(conditions.agent->position, conditions.to) && conditions.isMoveToSet;
	}
};

struct FindFood
{
	static void Execute(MoveToState& conditions, FindFoodState& foodConditions)
	{
		foodConditions.eaten = false;

		//can check previous positions?
		if (!foodConditions.prevFoodPositions.empty())
		{
			foodConditions.nextToCheck = foodConditions.prevFoodPositions[0];
			conditions.to = foodConditions.nextToCheck;
			conditions.isMoveToSet = true;
			conditions.from = conditions.agent->position;
			conditions.path = AStar::toFindPath(conditions.from, conditions.to);
			return;
		}

		//loop to find next unsearch patrol point
		for (std::pair<glm::vec2, bool>& p : foodConditions.patrolPoints)
		{
			if (foodConditions.nextToCheck == p.first)
			{
				p.second = true;
			}

			if (!p.second)
			{
				foodConditions.nextToCheck = p.first;
				foodConditions.nextToCheck = foodConditions.nextToCheck;
				conditions.to = foodConditions.nextToCheck;
				conditions.isMoveToSet = true;
				conditions.from = conditions.agent->position;
				conditions.path = AStar::toFindPath(conditions.from, conditions.to);
				break;
			}
		}
	}

	static bool IsValid(MoveToState& conditions, FindFoodState& foodConditions)
	{
		return !foodConditions.isFoodFound && !conditions.isMoveToSet && foodConditions.foundFoodRef == nullptr;
	}
};

struct EatFood
{
	static void Execute(MoveToState& conditions, FindFoodState& foodConditions)
	{
		if (conditions.agent->needs.hungerVal < 80)
		{
			foodConditions.foundFoodRef->EatFrom(5);
		}
		else
		{
			foodConditions.eaten = true;
		}
	}

	static bool IsValid(MoveToState& conditions, FindFoodState& foodConditions)
	{
		return foodConditions.foundFoodRef->canEat && foodConditions.isFoodFound && ComparePositions(conditions.agent->position, foodConditions.foundFoodRef->position);
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

static bool FoodGoalComplete(MoveToState& state, FindFoodState& foodState)
{
	if (foodState.isFoodFound && foodState.eaten) //is hunger value bigger now?
	{
		return true;
	}
	return false;
}