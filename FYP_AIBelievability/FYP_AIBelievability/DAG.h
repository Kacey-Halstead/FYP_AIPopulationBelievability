#pragma once
#include "Action.h"

struct DagNode
{
	Action* action;

	std::vector<DagNode*> children;

	DagNode(Action* a)
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

	std::vector<DagNode*> CreateNodes(std::vector<Action*>&& actions)
	{
		std::vector<DagNode*> newNodes;
		newNodes.reserve(actions.size());
		for (Action* a : actions)
		{			
			newNodes.push_back(allActions.emplace_back(new DagNode(a)));
		}
		return newNodes;
	}

	DagNode* FindPlan(actionIDs actionIndex, States& states)
	{
		DagNode* current = FindNode(actionIndex);
		if (current == nullptr) return nullptr;

		//if current complete then return
		ActionProgress progress = current->action->isValidFunc(states).first;
		if (progress != InProgress)
		{
			if (progress == Impossible)
				return FindPlan(GOAL_WANDER, states);
			else
				return nullptr;
		}

		while (!current->children.empty())
		{
			//if any children valid, return
			bool complete = true;
			bool impossible = true;
			int highestPriority = -1;
			DagNode* highest = current;
			for (auto child : current->children)
			{
				auto [state, prio] = child->action->isValidFunc(states);
				if (state == Impossible)
				{
					complete = false;
					continue;
				}

				impossible = false;

				if (state == InProgress && prio > highestPriority)
				{
					if(prio != 0)
						complete = false;
					highest = child;
					highestPriority = prio;
				}
			}
			if (impossible) return FindPlan(GOAL_WANDER, states);

			if (complete)
				break;

			current = highest;
		}
		return current;
	}

	DagNode* FindNode(actionIDs actionIndex)
	{
		for (DagNode* n : allActions)
		{
			if (n->action->ID == actionIndex)
			{
				return n;
			}
		}
		return nullptr;
	}

	std::vector<DagNode*> allActions;
};


