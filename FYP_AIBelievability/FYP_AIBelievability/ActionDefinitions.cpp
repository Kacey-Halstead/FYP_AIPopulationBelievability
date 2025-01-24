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
		return !states.foodState.foundFoodRef && !states.foodState.complete && !states.moveState.isMoveToSet;
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
		if (states.moveState.agent->needs.hungerVal < 80)
		{
			if (states.foodState.foundFoodRef->EatFrom(10))
			{
				states.moveState.agent->needs.hungerVal += 5;
			}
		}
		else
		{
			states.foodState.complete = true;
			states.foodState.foundFoodRef = nullptr;
		}
	}

	static bool IsValid(States& states)
	{
		return states.foodState.foundFoodRef != nullptr && !states.moveState.isMoveToSet && !states.foodState.complete;
	}
};

//WATER
struct FindWater
{
	static void Execute(States& states)
	{
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
		states.moveState.path = AStar::toFindPath(states.moveState.agent->position, states.moveState.to);
	}

	static bool IsValid(States& states)
	{
		return !states.waterState.waterRefSet && !states.waterState.complete && !states.moveState.isMoveToSet;
	}
};

struct DrinkWater
{
	static void Execute(States& states)
	{
		if (states.moveState.agent->needs.thirstVal < 80)
		{
			states.moveState.agent->DrinkWater(10);
		}
		else
		{
			states.waterState.complete = true;
			states.waterState.waterRefSet = false;
		}
	}

	static bool IsValid(States& states)
	{
		return states.waterState.waterRefSet && !states.moveState.isMoveToSet && !states.waterState.complete;
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


namespace Actions
{

	extern std::vector<Action> foodActions = {
	std::make_pair(std::make_pair(FindFood::Execute, FindFood::IsValid), FOODACTION),
	std::make_pair(std::make_pair(EatFood::Execute, EatFood::IsValid), FOODACTION2)
	};

	extern std::vector<Action> waterActions = {
	std::make_pair(std::make_pair(FindWater::Execute, FindWater::IsValid), WATERACTION),
	std::make_pair(std::make_pair(DrinkWater::Execute, DrinkWater::IsValid), WATERACTION2)
	};



	extern std::vector<DAG> dags = {
		DAG(foodActions),
		DAG(waterActions)
	};

	DAG GetDAG(ActionIndexes index)
	{
		return dags[index];
	}

	std::vector<Action> GetActions(ActionIndexes index)
	{
		switch (index)
		{
		case FOOD:
			return foodActions;
		case WATER:
			return waterActions;
		}

		return std::vector<Action>();
	}
}