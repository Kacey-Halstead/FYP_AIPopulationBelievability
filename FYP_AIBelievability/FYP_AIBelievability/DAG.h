#pragma once

#include "Action.h"

template<typename... Structs>
struct node
{
	int id;

	ActionWithStructs<Structs...> action;

	std::vector<ActionWithStructs<Structs...>> children;
};

template<typename... Structs>
class DAG
{
public:

	DAG(std::vector<ActionWithStructs<Structs...>> allActionsOfType)
	{
		int counter = 0;
		for (auto Action : allActionsOfType)
		{
			node<Structs...> n{};
			n.id = counter++;
			n.action = Action;
			allActions.push_back(n);
		}
	}


	~DAG()
	{

	}


	void AddRelation(ActionWithStructs<Structs...> parent, ActionWithStructs<Structs...> child)
	{
		for (node n : allActions)
		{
			if (n.id == parent.second)
			{
				n.children.push_back(child);
			}
		}
	}

private:
	std::vector<node<Structs...>> allActions;
};


