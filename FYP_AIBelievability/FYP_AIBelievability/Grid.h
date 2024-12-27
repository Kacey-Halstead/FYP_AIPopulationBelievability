#pragma once
#include <vector>
#include "Tile.h"

using namespace std;
class Grid
{
public:
	int sizeX;
	int sizeY;
	vector<vector<Tile*>> Tiles;
	vector<char> allTypes;

	Grid(int x, int y, vector<char> types);
	~Grid();

	Tile* SmallestEntropy();
	bool IsInGrid(int point, SDL_Point dir);

private:
	void GridInit();
};

