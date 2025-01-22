#pragma once

#include "Action.h"

template<typename... Structs>
struct DAGNode
{
	ActionWithStructs<Structs...> action;

	bool test()
	{
		return action.second(Structs...);
	}

	void apply(Structs... states)
	{
		action.first(states);
	}

	//will need some sort of cost eventually
};

template<typename... Structs>
class DAG
{
public:
	DAG();
	~DAG();


	void CreateGraph(std::vector<ActionWithStructs<Structs...>> allActionsofType);

private:
	std::vector<DAGNode<Structs...>> allNodes;
};


