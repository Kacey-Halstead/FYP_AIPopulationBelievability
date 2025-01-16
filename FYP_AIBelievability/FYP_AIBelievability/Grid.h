#pragma once
#include <vector>
#include "Tile.h"
#include "Commons.h"
#include <SDL.h>
#include <SDL_image.h>

using namespace std;
class Grid
{
public:
	int sizeX;
	int sizeY;
	SDL_Point tileSize;
	vector<vector<Tile*>> Tiles;
	std::array<char, numberOfTypes> allTypes;
	std::vector<SDL_Rect> rects;

	Grid(int x, int y, std::array<char, numberOfTypes> types);
	~Grid();

	void CreateRects(SDL_Window* SDLWindowRef);
	Tile* SmallestEntropy();
	bool IsInGrid(SDL_Point point, SDL_Point dir);
private:
	void GridInit();
};

