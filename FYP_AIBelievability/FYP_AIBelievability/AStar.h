#pragma once
#include "Grid.h"
#include "Tile.h"
#include <SDL.h>
#include <cmath>
#include <algorithm>

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

	friend bool operator == (const Node& a, const Node& b)
	{
		return a.tile == b.tile;
	}
};

class AStar
{
public:
	AStar();
	void Findpath(Node start, Node end);
	float Heuristic_Manhatten(Node start, Node end);
	float Heuristic_Manhatten(Tile start, Tile end);
	void ResetTiles(vector<vector<Tile*>> toReset);

	std::vector<Node> path;
	bool CanCutCorners = true;
	bool AllowDiagonal = true;


private:
	int maxPathCount = 10;

	void SetPath(Node* end);
	void DrawPath();
	bool DoesContainNode(std::vector<Node> list, Tile tile);
	float Magnitude(SDL_Point s);
	Node* GetNodeInList(std::vector<Node> list, Tile tile);
	Node* GetCheapestNode(std::vector<Node> openList);
	Tile* GetNeighbour(int index, Tile* current);
	
	
};

