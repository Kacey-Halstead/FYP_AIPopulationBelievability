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
	std::vector<Node> FindPath(Tile* start, Tile* end);
	std::vector<Node> FindPath(const glm::vec2& start, const glm::vec2& end);

	void SetPath(Node* end);

	void SetGridRef(Grid* grid);
};

