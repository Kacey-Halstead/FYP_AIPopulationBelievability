#pragma once
#include <SDL.h>
#include <functional>

#include "Tile.h"
#include "AStar.h"
#include "Commons.h"
#include "States.h"
#include "RandomGenerator.h"
#include "ActionDefinitions.h"
#include "ImGuiImplementation.h"

class DAG;

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

	std::string Getname(ActionIDs IDs)
	{
		switch (IDs)
		{
		case FOODACTION:
			return "Find Food";
		case FOODACTION2:
			return "Eat Food";
		case WATERACTION:
			return "Find Water";
		case WATERACTION2:
			return "Drink Water";
		case WANDER1:
			return "Wander";
		}

		return " ";
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
					ImGui_Implementation::action = Getname(ID);
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
	static bool GoalComplete(States& state)
	{
		if (ComparePositions(state.moveState.agent->position, state.moveState.to, 5))
		{
			return true;
		}
		return false;
	}


	static bool FoodGoalComplete(States& states)
	{
		if (states.moveState.agent->needs.hungerVal > 80)
		{
			return true;
		}
		return false;
	}


	static bool ThirstGoalComplete(States& states)
	{
		if (states.moveState.agent->needs.thirstVal > 80)
		{
			return true;
		}
		return false;
	}

	//decide goal

	static std::pair<std::pair<IsGoalComplete, std::vector<Action>>, DAG*> PickGoal(States& states);
};