#include "States.h"
#include "Agent.h"

bool MoveToState::IsComplete()
{
	if (ComparePositions(agent->position, to, 5))
	{
		return true;
	}

	return false;
}

bool FindFoodState::IsComplete()
{
	if (foundFoodRef != nullptr)
	{
		complete = false;
		return true;
	}
	return false;
}

bool FindWaterState::IsComplete()
{
	if (waterRefSet)
	{
		complete = false;
		return true;
	}
	return false;
}

bool FindState::IsComplete()
{
	if (isFound)
	{
		return true;
	}
	return false;
}