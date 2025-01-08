#pragma once
#include <vector>
#include <array>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "Grid.h"
#include "Tile.h"
#include "TextureManager.h"

struct Rules
{
	std::vector<std::array<char, 3>> ruleVec = {

		{'L', 'S', 'U'},
		{'L', 'S', 'D'},
		{'L', 'S', 'L'},
		{'L', 'S', 'R'},

		{'S', 'L', 'U'},
		{'S', 'L', 'D'},
		{'S', 'L', 'L'},
		{'S', 'L', 'R'}
	};
};

enum directions
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

class WFC
{
public:
	WFC(Grid* grid);
	~WFC();

	void WFCReset();
	void RenderWFC(SDL_Renderer* renderer);
	void WFCBody();
	void CreateRects(SDL_Window* SDLWindowRef);
	bool IsInTile(SDL_Point p, Tile t);
	vector<vector<Tile*>> GetTiles();

private:
	int gridX = 0;
	int gridY = 0;

	Grid* gridRef = nullptr;

	std::vector<SDL_Rect> rects;
	Rules rules{};

	std::vector<SDL_Point> offsets = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; //down, up, left, right

	bool IsInGrid(const SDL_Point& pos, const SDL_Point& offset);
	void Evaluate(Tile* tile, directions dir);
	void ResetNeighbours(vector<Tile*> tiles);
	std::vector<char> GetTypeAndRules(char input, directions dir);
	void ChangeTileWeighting(Tile* tile);
	void FindAndErase(Tile* tile, char toFind);
	void CheckForEmptyTiles(Tile* tile);
};

SDL_Point operator *(const SDL_Point& a, const int& b);
