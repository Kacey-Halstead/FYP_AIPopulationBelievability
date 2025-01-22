#include "DAG.h"

template<typename ...Structs>
DAG<Structs...>::DAG()
{

}

template<typename ...Structs>
void DAG<Structs...>::CreateGraph(std::vector<ActionWithStructs<Structs...>> allActionsofType)
{


	for (const auto& [execute, valid] : allActionsofType)
	{
		if (valid(Structs...))
		{
			DAGNode node = new DAGNode();
			node.action = { execute, valid };

			allNodes.push_back(node);
		}
	}

	for (auto node : allNodes)
	{

	}
}
