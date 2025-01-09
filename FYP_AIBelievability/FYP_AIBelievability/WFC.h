#pragma once
#include <vector>
#include <array>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "Grid.h"
#include "Tile.h"
#include "TextureManager.h"

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

	std::array<int, 3> typeCounter = {}; //L, C, S

private:
	Grid* gridRef = nullptr;

	std::vector<SDL_Rect> rects;
	std::vector<SDL_Point> offsets = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; //down, up, left, right
	std::vector<char> GetTypeAndRules(char input, directions dir);

	bool IsInGrid(const SDL_Point& pos, const SDL_Point& offset);

	bool EveryTileHasType();


	void Evaluate(Tile* tile, directions dir);
	void ResetTiles(vector<Tile*> tiles);
	void ChangeTileWeighting(Tile* tile);
	void FindAndErase(Tile* tile, char toFind);
	void CheckForEmptyTiles(Tile* tile);
	void TypeIncrement(char typeToIncrement);


	std::vector<std::array<char, 3>> rules = {

	{'L', 'S', 'U'}, //L and S can never touch
	{'L', 'S', 'D'},
	{'L', 'S', 'L'},
	{'L', 'S', 'R'},

	{'S', 'L', 'U'},
	{'S', 'L', 'D'},
	{'S', 'L', 'L'},
	{'S', 'L', 'R'}
	};
};

SDL_Point operator *(const SDL_Point& a, const int& b);
