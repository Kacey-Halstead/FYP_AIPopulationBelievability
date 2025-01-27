#pragma once
#include "Action.h"

struct node
{
	Action* action;

	std::vector<node*> children;

	node(Action* a)
	{
		action = a;
	}
};

class DAG
{
public:

	DAG()
	{

	}

	~DAG()
	{
		for (int i = 0; i < allActions.size(); i++)
		{
			delete allActions[i];
		}
	}

	void CreateNode(std::vector<Action>* actions)
	{
		for (Action& a : *actions)
		{			
			allActions.emplace_back(new node(&a));
		}
	}

	node* FindPlan(ActionIDs action, States& states)
	{
		node* current = FindNode(action);
		if (current == nullptr) return nullptr;

		//if current valid then return
		if (current->action->first.second(states))
		{
			return nullptr;
		}

		while (!current->children.empty())
		{
			//if any children valid, return
			bool complete = true;
			for (auto child : current->children)
			{
				if (!child->action->first.second(states))
				{
					complete = false;
					current = child;
					break;
				}
			}

			if (complete)
				break;
		}
		return current;
	}

	node* FindNode(ActionIDs action)
	{
		for (node* n : allActions)
		{
			if (n->action->second == action)
			{
				return n;
			}
		}
		return nullptr;
	}

	std::vector<node*> allActions;
};


