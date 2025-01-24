#pragma once
#include <SDL.h>
#include <functional>

#include "Tile.h"
#include "AStar.h"
#include "Commons.h"
#include "States.h"
#include "RandomGenerator.h"
#include"ActionDefinitions.h"

enum ActionProgress
{
	InProgress,
	Complete,
	Impossible
};

//Goal Completion function (IsGoalComplete)
using IsGoalComplete = std::function<bool(States&)>;

//PLANNER
class Planner 
{
public:

	Planner()
	{

	}

	void SetPlan(IsGoalComplete goal, std::vector<Action>&& allActions)
	{
		isGoalComplete = goal;
		actions = allActions;
	}

	Planner(IsGoalComplete goal, std::vector<Action>&& allActions) //planner requires goal complete func and actions in plan
	{
		isGoalComplete = goal;
		actions = allActions;
	}

	std::pair<const ExecuteFunc*, ActionProgress> ActionSelector(States& states)
	{
		if (actions.empty()) return { nullptr, Impossible };

		if (!isGoalComplete(states))
		{
			//cycle through actions and decide action
			for (auto& [funcs, ID] : actions)
			{

				if (funcs.second(states))
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
	IsGoalComplete isGoalComplete;
	std::vector<Action> actions;
};

class Goals
{
public:
	Goals() = delete;


	//Goal Completion Functions
	static bool GoalComplete(MoveToState& state)
	{
		if (ComparePositions(state.agent->position, state.to))
		{
			return true;
		}
		return false;
	}


	static bool FoodGoalComplete(States& states)
	{
		if (states.foodState.complete)
		{
			return true;
		}
		return false;
	}


	static bool ThirstGoalComplete(States& states)
	{
		if (states.waterState.complete)
		{
			return true;
		}
		return false;
	}

	//decide goal

	static std::pair<IsGoalComplete, std::vector<Action>> PickGoal(States& states)
	{
		int index = states.moveState.agent->DecideOnGoal();

		switch (index)
		{
		case 0:

			return std::make_pair(FoodGoalComplete, Actions::GetActions(0));
			break;
		case 1:
			return std::make_pair(ThirstGoalComplete, Actions::GetActions(0)); //CHANGE TO THIRST ACTIONS
			break;
		}

		return std::make_pair(FoodGoalComplete, Actions::GetActions(0));
	}
};