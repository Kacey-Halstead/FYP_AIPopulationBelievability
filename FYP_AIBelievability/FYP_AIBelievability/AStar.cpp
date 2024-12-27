#include "AStar.h"

void AStar::Findpath(Node start, Node end)
{
	path.clear();

	std::vector<Node> OpenList; //not visited
	std::vector<Node> ClosedList; //visited

	Node* current = &Node(start.tile, nullptr, 0.0f, Heuristic_Manhatten(start, end));
	OpenList.emplace_back(current);

	int maxIteration = 0;

	//while current node not empty
	while (current != nullptr)
	{
		maxIteration++;
		if (maxIteration > maxPathCount)
		{

			break;
		}

		//if the current node is the end node, a path has been found.
		//if (current.node == end)
		//{
		//	//Debug.Log("Path found, start pos = " + start.transform.position + " - end pos = " + end.transform.position);
		//	SetPath(current);
		//	DrawPath(openList, closedList);
		//	return;
		//}
	}
}

float AStar::Heuristic_Manhatten(Node start, Node end)
{
	SDL_Point distance = end.tile.pos - start.tile.pos;
	float XDiff = abs(distance.x);
	float YDiff = abs(distance.y);
	return XDiff + YDiff;
}
