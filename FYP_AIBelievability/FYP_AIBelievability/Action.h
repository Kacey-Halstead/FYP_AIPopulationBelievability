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
	FOODACTION2
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

//PLANNER
template<typename... Structs>
class Planner 
{
public:

	Planner()
	{

	}

	void SetPlan(IsGoalComplete<Structs...> goal, std::vector<Action<Structs...>>&& allActions)
	{
		isGoalComplete = goal;
		actions = allActions;
	}

	Planner(IsGoalComplete<Structs...> goal, std::vector<Action<Structs...>>&& allActions) //planner requires goal complete func and actions in plan
	{
		isGoalComplete = goal;
		actions = allActions;
	}

	std::pair<const ExecuteFunc<Structs...>*, ActionProgress> ActionSelector(Structs&... states)
	{
		if (actions.empty()) return { nullptr, Impossible };

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
	std::vector<Action<Structs...>> actions;
};

//Decide on goal
//function to return goal and structs, evaluating agent needs



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

//FOOD
struct FindFood
{
	static void Execute(MoveToState& conditions, FindState& findState, FindFoodState& foodConditions)
	{
		//can check previous positions?
		if (!foodConditions.prevFoodPositions.empty())
		{
			findState.nextToCheck = foodConditions.prevFoodPositions[0];
			conditions.to = findState.nextToCheck;
			conditions.isMoveToSet = true;
			conditions.from = conditions.agent->position;
			conditions.path = AStar::toFindPath(conditions.agent->position, conditions.to);

			return;
		}

		//loop to find next unsearch patrol point
		for (std::pair<glm::vec2, bool>& p : findState.patrolPoints)
		{
			if (findState.nextToCheck == p.first)
			{
				p.second = true;
			}

			if (!p.second)
			{
				findState.nextToCheck = p.first;
				conditions.to = findState.nextToCheck;
				conditions.from = conditions.agent->position;
				conditions.isMoveToSet = true;
				conditions.path = AStar::toFindPath(conditions.agent->position, conditions.to);

				break;
			}
		}
	}

	static bool IsValid(MoveToState& conditions, FindState& findState, FindFoodState& foodConditions)
	{
		return !foodConditions.foundFoodRef && !findState.complete && !conditions.isMoveToSet;
	}
};

struct EatFood
{
	static void Execute(MoveToState& conditions, FindState& findState, FindFoodState& foodConditions)
	{
		if (conditions.agent->needs.hungerVal < 80)
		{
			if (foodConditions.foundFoodRef->EatFrom(10))
			{
				conditions.agent->needs.hungerVal += 5;
			}
		}
		else
		{
			findState.complete = true;
			foodConditions.foundFoodRef = nullptr;
		}
	}

	static bool IsValid(MoveToState& conditions, FindState& findState, FindFoodState& foodConditions)
	{
		return foodConditions.foundFoodRef != nullptr && !conditions.isMoveToSet && !findState.complete;
	}
};

//WATER
struct FindWater
{
	static void Execute(MoveToState& conditions, FindState& findState, FindWaterState& waterConditions)
	{
		//can check previous positions?
		if (!waterConditions.prevWaterPositions.empty())
		{
			findState.nextToCheck = waterConditions.prevWaterPositions[0];
			conditions.to = findState.nextToCheck;
			conditions.isMoveToSet = true;
			conditions.from = conditions.agent->position;
			conditions.path = AStar::toFindPath(conditions.agent->position, conditions.to);

			return;
		}

		//loop to find next unsearch patrol point
		for (std::pair<glm::vec2, bool>& p : findState.patrolPoints)
		{
			if (findState.nextToCheck == p.first)
			{
				p.second = true;
			}

			if (!p.second)
			{
				findState.nextToCheck = p.first;
				conditions.to = findState.nextToCheck;
				conditions.from = conditions.agent->position;
				conditions.isMoveToSet = true;
				conditions.path = AStar::toFindPath(conditions.agent->position, conditions.to);

				break;
			}
		}
	}

	static bool IsValid(MoveToState& conditions, FindState& findState, FindWaterState& waterConditions)
	{
		return !waterConditions.foundWaterRef && !findState.complete && !conditions.isMoveToSet;
	}
};

struct DrinkWater
{
	static void Execute(MoveToState& conditions, FindWaterState& waterConditions, FindState& findState)
	{
		if (conditions.agent->needs.hungerVal < 80)
		{
			if (waterConditions.foundWaterRef->EatFrom(10))
			{
				conditions.agent->needs.hungerVal += 5;
			}
		}
		else
		{
			findState.complete = true;
			waterConditions.foundWaterRef = nullptr;
		}
	}

	static bool IsValid(MoveToState& conditions, FindWaterState& waterConditions, FindState& findState)
	{
		return waterConditions.foundWaterRef != nullptr && !conditions.isMoveToSet && !findState.complete;
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

template<typename... Structs>
static bool FindGoalComplete(MoveToState& state, FindState& findState, Structs&...)
{
	if (findState.complete)
	{
		return true;
	}
	return false;
}
