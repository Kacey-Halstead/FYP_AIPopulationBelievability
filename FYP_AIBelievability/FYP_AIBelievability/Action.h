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

enum ActionIDs
{
	FOODACTION,
};

//Action - 2 functions (Execute and IsValid) 
template<typename... Structs>
using ExecuteFunc = std::function<void(Structs&...)>;

using IsValidReturnType = bool; // TODO: change from bool to weighting to compare action desirability

template<typename... Structs>
using IsValidFunc = IsValidReturnType(*)(Structs&...); //function pointer - smaller memory than using std::function

//action definition 
template<typename... Structs>
using Action = std::pair<std::pair<ExecuteFunc<Structs...>, IsValidFunc<Structs...>>, ActionIDs>;

//Goal Completion function (IsGoalComplete)
template<typename... Structs>
using IsGoalComplete = std::function<bool(Structs&...)>;

template<typename... Structs>
using ActionWithStructs = Action<Structs...>;

//PLANNER
template<typename... Structs>
class Planner 
{
public:

	Planner(IsGoalComplete<Structs...> goal, std::vector<ActionWithStructs<Structs...>>&& allActions) //planner requires goal complete func and actions in plan
	{
		isGoalComplete = goal;
		actions = allActions;
	}

	std::pair<const ExecuteFunc<Structs...>*, ActionProgress> ActionSelector(Structs&... states)
	{
		if (!isGoalComplete(states...))
		{
			//cycle through actions and decide action
			for (auto& [funcs, ID] : actions)
			{

				if (funcs.second(states...))
				{
					return std::make_pair(&funcs.first, InProgress);
				}
			}

			//no actions available and not complete
			return std::pair(nullptr, Impossible);
		}

		return std::pair(nullptr, Complete);
	}

private:
	IsGoalComplete<Structs...> isGoalComplete;
	std::vector<ActionWithStructs<Structs...>> actions;
};

//Defined Actions

template<typename... Structs> //so moveTo can be used in many plans
struct MoveTo
{

	static void Execute(MoveToState& conditions, Structs& ...)
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

		if (ComparePositions(conditions.agent->position, conditions.to) || conditions.path.empty())
		{
			conditions.isMoveToSet = false;
		}
	}

	static bool IsValid(MoveToState& conditions, Structs& ...)
	{
		return !ComparePositions(conditions.from, conditions.to) && conditions.isMoveToSet;
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
			conditions.isMoveToSet = true;
			conditions.from = conditions.agent->position;
			conditions.path = AStar::toFindPath(conditions.agent->position, conditions.to);
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
				conditions.to = foodConditions.nextToCheck;
				conditions.from = conditions.agent->position;
				conditions.isMoveToSet = true;
				conditions.path = AStar::toFindPath(conditions.agent->position, conditions.to);
				break;
			}
		}
	}

	static bool IsValid(MoveToState& conditions, FindFoodState& foodConditions)
	{
		return !foodConditions.foundFoodRef && !foodConditions.eaten && !conditions.isMoveToSet;
	}
};

struct EatFood
{
	static void Execute(MoveToState& conditions, FindFoodState& foodConditions)
	{
		if (conditions.agent->needs.hungerVal < 80)
		{
			foodConditions.foundFoodRef->EatFrom(5);
			conditions.agent->needs.hungerVal += 5;
			foodConditions.prevFoodPositions.push_back(foodConditions.foundFoodRef->position);
		}
		else
		{
			foodConditions.eaten = true;
			foodConditions.foundFoodRef = nullptr;
		}
	}

	static bool IsValid(MoveToState& conditions, FindFoodState& foodConditions)
	{
		return foodConditions.foundFoodRef != nullptr && !conditions.isMoveToSet && !foodConditions.eaten;
	}
};

struct Wander
{
	static void Execute(MoveToState& conditions)
	{
		std::uniform_int_distribution<> distrib(1, gridSizeX - 1);

		conditions.from = conditions.agent->position;

		while (true)
		{
			conditions.to.x = distrib(RandomGenerator::gen) * conditions.agent->GetGridRef()->tileSize.x;
			conditions.to.y = distrib(RandomGenerator::gen) * conditions.agent->GetGridRef()->tileSize.y;

			conditions.path = AStar::toFindPath(conditions.from, conditions.to);
			if (!conditions.path.empty())
			{
				conditions.isMoveToSet = true;
				break;
			}
		}
	}

	static bool IsValid(MoveToState& conditions)
	{
		return !conditions.isMoveToSet;
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
	if (foodState.eaten) 
	{
		return true;
	}
	return false;
}
