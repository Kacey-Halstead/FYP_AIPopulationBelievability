#pragma once
#include <vector>
#include <SDL.h>
#include "Tile.h"
#include "AStar.h"
#include "Agent.h"

class Action
{
protected:
	virtual void Effects(const void* conditions) = 0;
	virtual bool Execute(void* conditions) = 0;
	virtual bool IsValid(void* conditions) = 0;
};

class MoveTo : Action
{
public:
	void Effects(const MoveToState* conditions)
	{
		conditions->agent->position = SDL_FPoint(conditions->to->pos.x, conditions->to->pos.y);
	}

	bool Execute(MoveToState* conditions)
	{
		Node current = conditions->path.front();
		conditions->agent->Move(SDL_FPoint(current.tile->pos.x, current.tile->pos.y));
		if (SDL_Point(conditions->agent->position.x, conditions->agent->position.y) == conditions->to->pos)
		{
			conditions->path.erase(conditions->path.begin());
		}

		if (conditions->agent->GetTileFromPos(SDL_Point(conditions->agent->position.x, conditions->agent->position.y)) == conditions->to || conditions->path.empty())
		{
			return true;
		}
		return false;
	}

	bool IsValid(MoveToState* conditions)
	{
		conditions->path = AStar::Findpath(conditions->from, conditions->to);
		return conditions->path.size() > 0;
	}
};




//class BaseAction
//{
//public:
//	virtual void Effects(const void* conditions) = 0;
//	virtual void Execute(void* conditions) = 0;
//	virtual bool IsValid(void* conditions) = 0;
//};
//
//template<typename T>
//class Action
//{
//public:
//	virtual void Effects(const void* conditions) final
//	{
//		Effects(*reinterpret_cast<const T*>(conditions));
//	}
//
//	virtual void Execute(void* conditions) final
//	{
//		Execute(*reinterpret_cast<T*>(conditions));
//	}
//
//	virtual bool IsValid(void* conditions) final
//	{
//		return IsValid(*reinterpret_cast<T*>(conditions));
//	}
//
//private:
//	virtual void Effects(const T& conditions) = 0;
//	virtual void Execute(T& conditions) = 0;
//	virtual bool IsValid(T& conditions) = 0;
//};
//
//template<typename T>
//class Planner
//{
//public:
//	void ExecutePlan(std::vector<Action<T>*>& plan)
//	{
//		for (Action<T>* a : plan)
//		{
//			if (a->IsValid())
//			{
//				a->Execute();
//				a->Effects();
//			}
//		}
//	}
//};
//
//class MoveTo : public Action<MoveToState>
//{
//private:
//	void Effects(const MoveToState& conditions) override
//	{
//		conditions.agent->position = conditions.to->pos;
//	}
//
//	void Execute(MoveToState& conditions) override
//	{
//		while (!conditions.path.empty())
//		{
//			Node current = conditions.path.front();
//			if (conditions.agent->Move(current.tile->pos))
//			{
//				conditions.path.erase(conditions.path.begin());
//			}
//		}
//	}
//
//	bool IsValid(MoveToState& conditions) override
//	{
//		conditions.path = AStar::Findpath(conditions.from, conditions.to);
//		return conditions.path.size() > 0;
//	}
//};