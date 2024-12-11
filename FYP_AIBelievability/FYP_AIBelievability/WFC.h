#pragma once
#include <vector>
#include <array>
#include "Grid.h"
#include "Tile.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

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
	WFC(int GridX, int GridY, SDL_Window* window, SDL_Renderer* renderer);
	~WFC();

	static std::array<char, 3> GenerateRule(char a, char b, char c)
	{
		std::array<char, 3> rule = { a, b, c };
		return rule;
	}

	void RenderWFC(SDL_Renderer* renderer);

private:
	int gridX = 0;
	int gridY = 0;

	Grid* grid = nullptr;

	std::vector<std::string> fullTypes = { "LAND", "COAST", "SEA" };
	std::vector<char> charTypes;
	std::vector<SDL_Texture*> textures;
	std::vector<std::array<char, 3>> Rules;

	SDL_Window* SDLWindowRef = nullptr;

	void DefineRules();
	void WFCBody();
	void Evaluate(Grid* grid, Tile* tile, char dir);
	void ResetNeighbours(vector<Tile*> tiles);
	SDL_Texture* CreateTexture(const char* filePath, SDL_Renderer* renderer);
	std::vector<char> GetTypeAndRules(char input, char dir);

	void DeleteTextures();
	void CreateTextures(SDL_Renderer* renderer);
};

