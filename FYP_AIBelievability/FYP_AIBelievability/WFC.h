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

	static std::array<char, 3> GenerateRule(char a, char b, char c)
	{
		std::array<char, 3> rule = { a, b, c };
		return rule;
	}

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
	std::vector<std::array<char, 3>> Rules;

	void DefineRules();
	void Evaluate(Tile* tile, char dir);
	void ResetNeighbours(vector<Tile*> tiles);
	std::vector<char> GetTypeAndRules(char input, char dir);
};

