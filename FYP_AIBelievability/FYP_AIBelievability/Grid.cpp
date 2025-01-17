#include "Grid.h"
#include "iostream"

using namespace std;

Grid::Grid(int x, int y, std::array<char, numberOfTypes> types)
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
			Tile* tile = new Tile(glm::ivec2(x, y), index);
			newTiles.push_back(tile);
			index++;
		}
		Tiles.push_back(newTiles);
	}
}

void Grid::CreateRects(SDL_Window* SDLWindowRef)
{
	tileSize = { SDL_GetWindowSurface(SDLWindowRef)->w / sizeX, SDL_GetWindowSurface(SDLWindowRef)->h / sizeY };

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			SDL_Rect newRec{ x * (SDL_GetWindowSurface(SDLWindowRef)->w / sizeX), y * (SDL_GetWindowSurface(SDLWindowRef)->h / sizeY), tileSize.x, tileSize.y };
			Tiles[x][y]->SetWorldPos(newRec.x + tileSize.x / 2, newRec.y + tileSize.y / 2);
			rects.emplace_back(newRec);
		}
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
			if (Tiles[x][y]->typesAndWeights.size() < numOptions && Tiles[x][y]->GetType() == '0')
			{
				smallest.clear();
				smallest.push_back(Tiles[x][y]);
				numOptions = Tiles[x][y]->typesAndWeights.size();
			}
			else if (Tiles[x][y]->typesAndWeights.size() == numOptions && Tiles[x][y]->GetType() == '0')
			{
				smallest.push_back(Tiles[x][y]);
			}
		}
	}

	if (smallest.size() == 1)
	{
		return smallest[0];
	}
	else if(smallest.size() > 1)
	{
		int random = rand() % (smallest.size());
		return smallest[random];
	}
	else
	{
		for (int x = 0; x < sizeX; x++)
		{
			for (int y = 0; y < sizeY; y++)
			{
				if (Tiles[x][y]->GetType() == '0')
				{
					return Tiles[x][y];
				}
			}
		}
	}

	return nullptr;
}

bool Grid::IsInGrid(glm::ivec2 point, glm::ivec2 dir)
{
	glm::ivec2 pos = point + dir; // tile position

	//if pos within X and Y bounds of grid
	return (0 <= pos.x && pos.x < sizeX 
		&& 0 <= pos.y && pos.y < sizeY);
}
