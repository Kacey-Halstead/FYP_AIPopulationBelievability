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
	std::vector<char> GetTypeAndRules(char input, directions dir);

	bool IsInGrid(const glm::ivec2& pos, const glm::ivec2& offset);
	bool EveryTileHasType();


	void WFCReset();
	void WFCBody(Grid* grid);
	void Evaluate(Tile* tile, directions dir);
	void ChangeTileWeighting(Tile* tile);
	void FindAndErase(Tile* tile, char toFind);
	void CheckForEmptyTiles(Tile* tile);
	void TypeIncrement(char typeToIncrement);
	void SetEdgesAndCorners(char tileType);
	void PlaceFood();
	void PlaceDecor();
	std::vector<FoodSource>& GetFoodSources();
	void SetAgentsVec(std::vector<Agent>* agents);

	SDL_Point operator *(const SDL_Point& a, const int& b);
};

