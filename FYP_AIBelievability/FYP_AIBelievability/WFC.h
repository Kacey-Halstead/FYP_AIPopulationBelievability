#pragma once
#include <vector>
#include <array>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Grid.h"
#include "Tile.h"
#include "TextureManager.h"
#include "AStar.h"

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

	bool IsInTile(SDL_Point p, Tile t);

private:
	Grid* gridRef = nullptr;

	std::array<int, 3> typeCounter = {}; //L, C, S

	std::vector<glm::ivec2> offsets = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; //down, up, left, right
	std::vector<char> GetTypeAndRules(char input, directions dir);

	bool IsInGrid(const glm::ivec2& pos, const glm::ivec2& offset);
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
