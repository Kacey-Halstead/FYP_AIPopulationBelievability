#pragma once
#include "Grid.h"
#include "Tile.h"
#include <SDL.h>
#include <cmath>
#include <algorithm>
#include <memory>

#include "Commons.h"

struct Node {
	Tile* tile;
	Node* parent;
	float gcost;
	float hcost;
	float fcost;

	Node(Tile* t, Node* n, float g, float h)
	{
		UpdateInfo(t, n, g, h);
	}

	void UpdateInfo(Tile* t, Node* n, float g, float h)
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

namespace AStar
{
	std::vector<Node> Findpath(Tile* start, Tile* end);
	void ResetTiles(vector<vector<Tile*>>& toReset);
	void SetPath(Node* end);
	void DrawPath();

	std::vector<Node> toFindPath(glm::ivec2 start, glm::ivec2 end);

	bool DoesContainNode(const std::vector<Node*>& list, Tile* tile);

	float Heuristic_Manhatten(Tile* start, Tile* end);
	float Magnitude(SDL_Point s);

	Node* GetNodeInList(const std::vector<Node*>& list, Tile* tile);
	Node* GetCheapestNode(std::vector<Node*>& openList);

	Tile* GetNeighbour(int index, Tile* current);

	void SetGridRef(Grid* grid);
};

