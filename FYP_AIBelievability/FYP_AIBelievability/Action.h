#pragma once
#include <vector>

class Action
{
public:
	Action();
	~Action();
	virtual void Execute();
	bool IsValid();

private:

};

class MoveTo : Action
{
	struct preconditions
	{

	};


public:
	MoveTo();
	void Execute() override;

private:

};