#include "States.h"
#include "Agent.h"
#include "FoodSource.h"

bool MoveToState::IsComplete()
{
	if (ComparePositions(agent->position, to))
	{
		return true;
	}

	return false;
}

bool FindFoodState::IsComplete()
{
	if (ComparePositions(agent->position, foundFoodRef.position) && isFoodFound)
	{
		return true;
	}
	return false;
}
