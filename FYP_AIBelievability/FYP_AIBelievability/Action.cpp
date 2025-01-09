#include "Action.h"

Action::Action()
{
}

Action::~Action()
{
}

void Action::Execute()
{
	if (IsValid())
	{

	}
}

bool Action::IsValid()
{
	return false;
}


MoveTo::MoveTo() : Action()
{
}

void MoveTo::Execute()
{
	
}
