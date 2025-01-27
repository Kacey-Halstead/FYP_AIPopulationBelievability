#include "ActionDefinitions.h"
#include "DAG.h"

//Defined Actions

//FOOD
struct FindFood
{
	static void Execute(States& states)
	{
		//can check previous positions?
		if (!states.foodState.prevFoodPositions.empty())
		{
			states.findState.nextToCheck = states.foodState.prevFoodPositions[0];
			setNextCheck(states);
			return;
		}

		states.moveState.patrolIndex = (states.moveState.patrolIndex + 1) % states.findState.patrolPoints.size();
		states.findState.nextToCheck = states.findState.patrolPoints[states.moveState.patrolIndex];
		setNextCheck(states);

	}

	static bool IsValid(States& states)
	{
		return states.moveState.isMoveToSet && ComparePositions(states.moveState.agent->position, states.foodState.foundFoodRef->position, 20.0f);
	}

	static void setNextCheck(States& states)
	{
		states.moveState.to = states.findState.nextToCheck;
		states.moveState.isMoveToSet = true;
		states.moveState.from = states.moveState.agent->position;
		states.moveState.path = AStar::toFindPath(states.moveState.agent->position, states.moveState.to);
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct EatFood
{
	static void Execute(States& states)
	{
		if (states.foodState.foundFoodRef->EatFrom(100 - states.moveState.agent->needs.hungerVal))
		{
			states.moveState.agent->needs.hungerVal += (100 - states.moveState.agent->needs.hungerVal);
		}
		states.foodState.foundFoodRef = nullptr;
	}

	static bool IsValid(States& states)
	{
		return states.moveState.agent->needs.hungerVal > 90;
	}
};

// -------------------------------------------------------------------------------------------------------------------

//WATER
struct FindWater
{
	static void Execute(States& states)
	{
		states.moveState.from = states.moveState.agent->position;

		//can check previous positions?
		if (!states.waterState.prevWaterPositions.empty())
		{
			states.findState.nextToCheck = states.waterState.prevWaterPositions[0];

			setNextCheck(states);
			return;
		}		

		states.moveState.patrolIndex = (states.moveState.patrolIndex + 1) % states.findState.patrolPoints.size();
		states.findState.nextToCheck = states.findState.patrolPoints[states.moveState.patrolIndex];		
		setNextCheck(states);
	}

	static void setNextCheck(States& states)
	{
		states.moveState.to = states.findState.nextToCheck;
		states.moveState.isMoveToSet = true;
		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);
	}

	static bool IsValid(States& states)
	{
		return ComparePositions(states.moveState.agent->position, states.waterState.foundWaterRef, 20.0f);
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct DrinkWater
{
	static void Execute(States& states)
	{
		states.moveState.agent->DrinkWater((100-states.moveState.agent->needs.thirstVal));
	}

	static bool IsValid(States& states)
	{
		return states.moveState.agent->needs.thirstVal > 90;
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Wander
{
	static void Execute(States& states)
	{
		std::uniform_int_distribution<> distrib(1, gridSizeX - 1);

		states.moveState.from = states.moveState.agent->position;

		states.moveState.to.x = distrib(RandomGenerator::gen) * states.moveState.agent->GetGridRef()->tileSize.x;
		states.moveState.to.y = distrib(RandomGenerator::gen) * states.moveState.agent->GetGridRef()->tileSize.y;

		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);
		if (!states.moveState.path.empty())
		{
			states.moveState.isMoveToSet = true;
		}
	}

	static bool IsValid(States& states)
	{
		return states.moveState.isMoveToSet;
	}
};

// -------------------------------------------------------------------------------------------------------------------

namespace Actions
{

	std::vector<Action> foodActions = {
	std::make_pair(std::make_pair(EatFood::Execute, EatFood::IsValid), FOODACTION2),
	std::make_pair(std::make_pair(FindFood::Execute, FindFood::IsValid), FOODACTION)
	};

	std::vector<Action> waterActions = {
	std::make_pair(std::make_pair(DrinkWater::Execute, DrinkWater::IsValid), WATERACTION2),
	std::make_pair(std::make_pair(FindWater::Execute, FindWater::IsValid), WATERACTION)
	};

	std::vector<Action> wanderActions = {
	std::make_pair(std::make_pair(Wander::Execute, Wander::IsValid), WANDER1)
	};

	std::vector<Action>* GetActions(ActionIndexes index)
	{
		switch (index)
		{
		case FOOD:
			return &foodActions;
		case WATER:
			return &waterActions;
		case WANDER:
			return &wanderActions;
		}

		return &foodActions;
	}

	std::string Getname(ActionIDs IDs)
	{
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
	}
}