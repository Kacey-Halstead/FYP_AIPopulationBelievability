#pragma once
#include "Grid.h"
#include "Tile.h"
#include <SDL.h>
#include <cmath>
#include <algorithm>
#include <memory>

struct Node {
	Tile* tile;
	Node* parent;
	float gcost;
	float hcost;
	float fcost;

	Node(Tile* t, Node *n, float g, float h)
	{
		UpdateInfo(t, n, g, h);
	}

	void UpdateInfo(Tile* t, Node *n, float g, float h)
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
	AStar(Grid* grid);
	void Findpath(Tile* start, Tile* end);
	float Heuristic_Manhatten(const Tile* start, const Tile* end) const;
	void ResetTiles(vector<vector<Tile*>>& toReset);

	std::vector<Node> path;
	bool CanCutCorners = true;
	bool AllowDiagonal = true;


private:
	int maxPathCount = 100;
	Grid* gridRef = nullptr;

	void SetPath(Node* end);
	void DrawPath();
	bool DoesContainNode(const std::vector<Node*>& list, Tile* tile);
	float Magnitude(SDL_Point s);
	Node* GetNodeInList(const std::vector<Node*>& list, Tile* tile);
	Node* GetCheapestNode(std::vector<Node*>& openList);
	Tile* GetNeighbour(int index, Tile* current);
};

