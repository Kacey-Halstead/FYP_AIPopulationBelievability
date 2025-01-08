#include "Grid.h"
#include "iostream"

using namespace std;

Grid::Grid(int x, int y, vector<char> types)
{
	sizeX = x;
	sizeY = y;
	allTypes = types;
	GridInit();
}

Grid::~Grid()
{

}

void Grid::GridInit()
{
	int index = 0;
	//for every cell, define possible tiles
	for (int x = 0; x < sizeX; x++)
	{
		vector<Tile*> newTiles;
		for (int y = 0; y < sizeY; y++)
		{
			Tile* tile = new Tile(allTypes);
			tile->pos = { x, y };
			tile->index = index;
			newTiles.push_back(tile);
			index++;
		}
		Tiles.push_back(newTiles);
	}
}


Tile* Grid::SmallestEntropy()
{
	//find cell with smallest number of options
	vector<Tile*> smallest;
	int numOptions = 3;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			if (Tiles[x][y]->typesAndWeights.size() < numOptions && Tiles[x][y]->type == '0')
			{
				smallest.clear();
				smallest.push_back(Tiles[x][y]);
				numOptions = Tiles[x][y]->typesAndWeights.size();
			}
			else if (Tiles[x][y]->typesAndWeights.size() == numOptions && Tiles[x][y]->type == '0')
			{
				smallest.push_back(Tiles[x][y]);
			}
		}
	}

	if (smallest.size() == 1)
	{
		return smallest[0];
	}
	else if(smallest.size() > 0)
	{

		int random = rand() % (smallest.size());
		return smallest[random];
	}

	return nullptr;
}

bool Grid::IsInGrid(SDL_Point point, SDL_Point dir)
{
	SDL_Point pos = point + dir; // tile position

	//if pos within X and Y bounds of grid
	return (0 <= pos.x && pos.x < sizeX 
		&& 0 <= pos.y && pos.y < sizeY);
}
