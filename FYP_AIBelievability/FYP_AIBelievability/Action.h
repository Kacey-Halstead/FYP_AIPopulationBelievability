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



//Goal Completion function (IsGoalComplete)
using IsGoalComplete = std::function<bool(States&)>;

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

	static std::vector<Action>* PickGoal(States& states);
};