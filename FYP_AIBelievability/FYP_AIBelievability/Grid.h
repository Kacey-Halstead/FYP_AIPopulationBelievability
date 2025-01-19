#pragma once
#include <vector>
#include "Tile.h"
#include "Commons.h"
#include "Tile.h"
#include "TextureManager.h"

#include <SDL.h>
#include <SDL_image.h>
#include<glm/glm.hpp>

using namespace std;
class Grid
{
public:

	Grid(int x, int y, std::array<char, numberOfTypes> types);
	~Grid();

	void CreateRects(SDL_Window* SDLWindowRef);
	void RenderGrid(SDL_Renderer* renderer);

	Tile* GetTileFromPos(glm::vec2 pos);

	bool IsInGrid(glm::ivec2 point, glm::ivec2 dir);

	Tile* SmallestEntropy();

	inline glm::vec2 GetGridSize() { return glm::vec2(sizeX, sizeY); };

	glm::ivec2 tileSize;
	vector<vector<Tile*>> Tiles;
	std::vector<SDL_Rect> rects;

private:
	int sizeX;
	int sizeY;

	std::array<char, numberOfTypes> allTypes;

	void GridInit();
};

