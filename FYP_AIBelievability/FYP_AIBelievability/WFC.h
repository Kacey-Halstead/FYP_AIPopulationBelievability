#pragma once
#include <vector>
#include <array>
#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>

#include "RandomGenerator.h"
#include "Grid.h"
#include "Tile.h"
#include "AStar.h"
#include <bitset>

class FoodSource;
class Agent;

enum directions
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

namespace WFC
{
	void WFCReset();

	std::vector<FoodSource>& GetFoodSources();
	void SetAgentsVec(std::vector<Agent>* agents);
	void SetGridRef(Grid* grid);

	SDL_Point operator *(const SDL_Point& a, const int& b);
};

