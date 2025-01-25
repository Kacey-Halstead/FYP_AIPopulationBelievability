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

		//loop to find next unsearch patrol point
		for (std::pair<glm::vec2, bool>& p : states.findState.patrolPoints)
		{
			if (states.findState.nextToCheck == p.first)
			{ 
				p.second = true;
			}

			if (!p.second)
			{
				states.findState.nextToCheck = p.first;
				setNextCheck(states);

				break;
			}
		}
	}

	static bool IsValid(States& states)
	{
		return !states.foodState.foundFoodRef && !states.moveState.isMoveToSet;
	}

	static void setNextCheck(States& states)
	{
		states.moveState.to = states.findState.nextToCheck;
		states.moveState.isMoveToSet = true;
		states.moveState.from = states.moveState.agent->position;
		states.moveState.path = AStar::toFindPath(states.moveState.agent->position, states.moveState.to);
	}
};

struct EatFood
{
	static void Execute(States& states)
	{
		if (states.foodState.foundFoodRef->EatFrom(100 - states.moveState.agent->needs.hungerVal))
		{
			states.moveState.agent->needs.hungerVal += (100 - states.moveState.agent->needs.hungerVal);
		}

		states.findState = {};
		states.foodState = {};
		states.foodState.foundFoodRef = nullptr;
	}

	static bool IsValid(States& states)
	{
		return states.foodState.foundFoodRef != nullptr && !states.moveState.isMoveToSet;
	}
};

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

		//loop to find next unsearch patrol point
		for (std::pair<glm::vec2, bool>& p : states.findState.patrolPoints)
		{
			if (states.findState.nextToCheck == p.first)
			{
				p.second = true;

				//if all patrol points exhausted
				if (p.first == states.findState.patrolPoints[4].first)
				{
					states.findState.patrolPoints = {};
				}
			}

			if (!p.second)
			{
				states.findState.nextToCheck = p.first;
				setNextCheck(states);
				break;
			}
		}
	}

	static void setNextCheck(States& states)
	{
		states.moveState.to = states.findState.nextToCheck;
		states.moveState.isMoveToSet = true;
		states.moveState.from = states.moveState.agent->position;
		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);
	}

	static bool IsValid(States& states)
	{
		return !states.waterState.waterRefSet && !states.moveState.isMoveToSet;
	}
};

struct DrinkWater
{
	static void Execute(States& states)
	{
		states.moveState.agent->DrinkWater((100-states.moveState.agent->needs.thirstVal));
		states.findState = {};
		states.waterState = {};
	}

	static bool IsValid(States& states)
	{
		return states.waterState.waterRefSet && !states.moveState.isMoveToSet;
	}
};

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
		return !states.moveState.isMoveToSet;
	}
};


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

	extern std::vector<DAG> dags = {};

	DAG* GetDAG(ActionIndexes index)
	{
		return &dags[index];
	}

	std::vector<Action> GetActions(ActionIndexes index)
	{
		switch (index)
		{
		case FOOD:
			return foodActions;
		case WATER:
			return waterActions;
		case WANDER:
			return wanderActions;
		}

		return std::vector<Action>();
	}
}