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
	if (foundFoodRef != nullptr || !prevFoodPositions.empty())
	{
		return true;
	}
	return false;
}

bool FindWaterState::IsComplete()
{
	if (foundWaterRef != nullptr || !prevWaterPositions.empty())
	{
		return true;
	}
	return false;
}

bool FindState::IsComplete()
{
	if (isFound && complete)
	{
		return true;
	}
	return false;
}
