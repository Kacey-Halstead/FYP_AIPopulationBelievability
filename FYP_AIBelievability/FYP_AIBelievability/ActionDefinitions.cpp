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
			states.moveState.to = states.foodState.prevFoodPositions[0].first;
			states.foodState.foundFoodRef = states.foodState.prevFoodPositions[0].second;
			setNextCheck(states);
		}
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.foodState.prevFoodPositions.empty())
		{
			return { ActionProgress::Impossible, 1 };
		}
		else if(states.foodState.foundFoodRef != nullptr && ComparePositions(states.moveState.agent->position, states.foodState.foundFoodRef->position, 1.0f))
		{
			return { ActionProgress::Complete, 1 };
		}

		return { ActionProgress::InProgress, 1 };
	}

	static void setNextCheck(States& states)
	{
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
			states.moveState.agent->needs.hungerVal = 100;
		}
		else
		{
			states.foodState.prevFoodPositions.erase(states.foodState.prevFoodPositions.begin());
		}

		states.foodState.foundFoodRef = nullptr;
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.moveState.agent->needs.hungerVal > 60)
			return { ActionProgress::Complete, 1 };
		else
			return { ActionProgress::InProgress, 1 };
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
			states.waterState.foundWaterRef = states.waterState.prevWaterPositions[0];
			states.moveState.to = states.waterState.prevWaterPositions[0];
			states.waterState.waterRefSet = true;
			setNextCheck(states);
		}		
	}

	static void setNextCheck(States& states)
	{
		states.moveState.isMoveToSet = true;
		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);

	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if (states.waterState.prevWaterPositions.empty())
		{
			return { ActionProgress::Impossible, 1 };
		}
		else if (states.waterState.waterRefSet && ComparePositions(states.moveState.agent->position, states.waterState.foundWaterRef, 1.0f))
		{
			return { ActionProgress::Complete, 1 };
		}

		return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct DrinkWater
{
	static void Execute(States& states)
	{
		states.moveState.agent->DrinkWater((100-states.moveState.agent->needs.thirstVal));
		states.waterState.waterRefSet = false;
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		if(states.moveState.agent->needs.thirstVal > 60)
			return { ActionProgress::Complete, 1 };
		else
			return { ActionProgress::InProgress, 1 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Wander
{
	static void Execute(States& states)
	{
		std::uniform_int_distribution<> distrib(0, gridSizeX - 1);

		states.moveState.from = states.moveState.agent->position;

		states.moveState.to.x = distrib(RandomGenerator::gen);
		states.moveState.to.y = distrib(RandomGenerator::gen);

		states.moveState.path = AStar::toFindPath(states.moveState.from, states.moveState.to);
		if (!states.moveState.path.empty())
		{
			states.moveState.isMoveToSet = true;
		}
	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		return { ActionProgress::InProgress, 0 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Socialise
{
	static void Execute(States& states)
	{

	}

	static void setNextCheck(States& states)
	{

	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		return { ActionProgress::Impossible, 0 };
	}
};

// -------------------------------------------------------------------------------------------------------------------

struct Fight
{
	static void Execute(States& states)
	{

	}

	static void setNextCheck(States& states)
	{

	}

	static std::pair<ActionProgress,int> IsValid(States& states)
	{
		return { ActionProgress::Impossible, 0 };
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