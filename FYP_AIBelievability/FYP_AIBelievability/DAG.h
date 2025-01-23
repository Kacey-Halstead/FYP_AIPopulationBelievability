#pragma once

#include "Action.h"

template<typename... Structs>
struct node
{
	ActionIDs id;

	Action<Structs...> action;

	std::vector<node<Structs...>> children;
};

template<typename... Structs>
class DAG
{
public:

	DAG(std::vector<Action<Structs...>> allActionsOfType)
	{
		for (auto Action : allActionsOfType)
		{
			node<Structs...> n{};
			n.action = Action;
			n.id = Action.second;
			allActions.push_back(n);
		}
	}

	void AddRelation(Action<Structs...>* parent, Action<Structs...>* child)
	{
		node<Structs...>* parentNode = FindNode(parent);
		if (parentNode != nullptr)
		{
			node<Structs...>* childNode = FindNode(child);

			if (childNode != nullptr)
			{
				parentNode->children.push_back(*childNode);
			}
		}
	}

	bool FindPlan(Action<Structs...>* action, Structs... states)
	{
		node<Structs...>* parentNode = FindNode(action);
		if (parentNode == nullptr) return false;

		//if node is valid, return

		if (parentNode->action.first.second(states...))
		{
			comprisedPlan.push_back(*parentNode);
			return true;
		}
		else
		{
			if (parentNode->children.empty()) return false;

			//try all children until valid action found
			for (node<Structs...>& child : parentNode->children)
			{
				if (FindPlan(&child.action, states...))
				{
					comprisedPlan.push_back(child);
					return true;
				}
			}
			return false;
		}
		return false;
	}

	node<Structs...>* FindNode(Action<Structs...>* toFind)
	{
		for (node<Structs...>& n : allActions)
		{
			if (n.id == toFind->second)
			{
				return &n;
			}
		}
		return nullptr;
	}

	std::vector<Action<Structs...>> GetAction()
	{
		std::vector<Action<Structs...>> actions;

		for (node<Structs...> vecNode : comprisedPlan)
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
	std::vector<node<Structs...>> allActions;
	std::vector<node<Structs...>> comprisedPlan;

};


