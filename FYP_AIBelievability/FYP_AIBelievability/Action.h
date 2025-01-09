#pragma once
#include <vector>
#include <SDL.h>
#include "Tile.h"
#include "AStar.h"
#include "Agent.h"

template<typename preconditions>
class Action
{
public:
	virtual void Effects(const preconditions& conditions) = 0;
	virtual void Execute(preconditions& conditions) = 0;
	virtual bool IsValid(preconditions& conditions) = 0;

private:

};

template<typename state>
class Planner
{
public:
	void ExecutePlan(std::vector<Action<state>*>& plan)
	{
		for (Action<state>* a : plan)
		{
			if (a->IsValid())
			{
				a->Execute();
				a->Effects();
			}
		}
	}
};

struct ToMoveState
{
	Tile* to;
	Tile* from;
	Agent* agent;
	std::vector<Node> path;
};

class MoveTo : public Action<ToMoveState>
{
public:
	void Effects(const ToMoveState& conditions) override
	{
		conditions.agent->position = conditions.to->pos;
	}

	void Execute(ToMoveState& conditions) override
	{
		while (!conditions.path.empty())
		{
			Node current = conditions.path.front();
			if (conditions.agent->Move(current.tile->pos))
			{
				conditions.path.erase(conditions.path.begin());
			}
		}
	}

	bool IsValid(ToMoveState& conditions) override
	{
		conditions.path = AStar::Findpath(conditions.from, conditions.to);
		return conditions.path.size() > 0;
	}
};