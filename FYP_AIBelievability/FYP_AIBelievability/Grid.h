#pragma once
#include <vector>
#include "Tile.h"
#include "Commons.h"

using namespace std;
class Grid
{
public:
	int sizeX;
	int sizeY;
	vector<vector<Tile*>> Tiles;
	std::array<char, numberOfTypes> allTypes;

	Grid(int x, int y, std::array<char, numberOfTypes> types);
	~Grid();

	Tile* SmallestEntropy();
	bool IsInGrid(SDL_Point point, SDL_Point dir);

private:
	void GridInit();
};

