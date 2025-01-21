#include "States.h"
#include "Agent.h"

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
	if (isFoodFound && eaten)
	{
		return true;
	}
	return false;
}
