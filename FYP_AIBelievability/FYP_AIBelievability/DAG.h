#pragma once
#include "Action.h"

struct node
{
	Action action;

	std::vector<node> children;
};

class DAG
{
public:

	DAG(std::vector<Action> allActionsOfType)
	{
		for (auto Action : allActionsOfType)
		{
			node n{};
			n.action = Action;
			allActions.push_back(n);
		}
	}

	void AddRelation(Action* parent, Action* child)
	{
		node* parentNode = FindNode(parent);
		if (parentNode != nullptr)
		{
			node* childNode = FindNode(child);

			if (childNode != nullptr)
			{
				parentNode->children.push_back(*childNode);
			}
		}
	}

	bool FindPlan(Action* action, States& states)
	{
		node* parentNode = FindNode(action);
		if (parentNode == nullptr) return false;

		//if node is valid, return

		if (parentNode->action.first.second(states))
		{
			comprisedPlan.push_back(*parentNode);
			return true;
		}
		else
		{
			if (parentNode->children.empty()) return false;

			//try all children until valid action found
			for (node& child : parentNode->children)
			{
				if (FindPlan(&child.action, states))
				{
					comprisedPlan.push_back(child);
					return true;
				}
			}
			return false;
		}
		return false;
	}

	node* FindNode(Action* toFind)
	{
		for (node& n : allActions)
		{
			if (n.action.second == toFind->second)
			{
				return &n;
			}
		}
		return nullptr;
	}

	std::vector<Action> GetAction()
	{
		std::vector<Action> actions;

		for (node vecNode : comprisedPlan)
		{
			actions.push_back(vecNode.action);
		}

		return actions;
	}

	void ClearPlan()
	{
		comprisedPlan.clear();
	}

private:
	std::vector<node> allActions;
	std::vector<node> comprisedPlan;

};


