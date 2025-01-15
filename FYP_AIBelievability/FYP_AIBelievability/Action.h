#pragma once
#include <vector>
#include <SDL.h>
#include "Tile.h"
#include "AStar.h"
#include "Commons.h"
#include "Agent.h"
#include <functional>

enum ActionProgress
{
	InProgress,
	Complete,
	Impossible
};

template<typename... structs>
struct Action
{
public:
	virtual void Execute(structs&... conditions) {};

	virtual bool IsValid(structs&... conditions) { return false; };
};

template<typename... Structs>
using IsGoalComplete = std::function<bool(Structs&...)>; 

template<typename... Structs>
struct goal
{
	IsGoalComplete<Structs&...> isComplete;//tests goal completion

	std::vector<Action<Structs&...>> goalActions;//vector of actions with same structs
};

template<typename... Structs>
class Planner
{
public:
	Planner(goal<Structs&...> passedGoal) //list of all actions in plan
	{
		goalToExecute = passedGoal;
	}

	std::pair<Action<Structs&...>*, ActionProgress> ActionSelector(Structs&... states)
	{
		if (!GoalComplete(states...))
		{
			//cycle through actions and decide action
			for (Action<Structs&...> action : goalToExecute.goalActions)
			{
				if (action.IsValid(states...))
				{
					return std::pair(&action, InProgress);
				}
			}

			//no actions available and not complete
			return std::pair(nullptr, Impossible);
		}

		return std::pair(nullptr, Complete);
	}
private:
	goal<Structs&...> goalToExecute;
};

struct MoveTo : public Action<MoveToState&>
{
	void Execute(MoveToState& conditions) override
	{
		conditions.agent->Move(conditions.to);
	}

	bool IsValid(MoveToState& conditions) override
	{
		return !conditions.agent->ComparePositions(conditions.agent->position, conditions.to);
	}
};

static bool GoalComplete(MoveToState state)
{
	if (state.agent->ComparePositions(state.agent->position, state.to))
	{
		return true;
	}
	return false;
}