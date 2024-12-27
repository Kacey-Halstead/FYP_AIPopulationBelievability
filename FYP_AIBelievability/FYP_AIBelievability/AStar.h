#pragma once
#include "Grid.h"
#include "Tile.h"
#include <SDL.h>
#include <cmath>

struct Node {
	Tile tile;
	struct Node *parent;
	float gcost;
	float hcost;
	float fcost;

	Node(Tile t, Node *n, float g, float h)
	{
		UpdateInfo(t, n, g, h);
	}

	void UpdateInfo(Tile t, Node *n, float g, float h)
	{
		tile = t;
		parent = n;
		gcost = g;
		hcost = h;
		fcost = g + h;
	}
};

class AStar
{
public:
	void Findpath(Node start, Node end);
	float Heuristic_Manhatten(Node start, Node end);

	std::vector<Node> path;

private:
	int maxPathCount = 10;
};

