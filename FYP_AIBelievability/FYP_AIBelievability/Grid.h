#pragma once
#include <vector>
#include "Commons.h"
#include "TextureManager.h"

#include <SDL.h>
#include <SDL_image.h>
#include<glm/glm.hpp>

class Tile;

class Grid
{
public:
	Grid(std::array<char, numberOfTypes> types);
	~Grid();

	void RenderGrid(SDL_Renderer* renderer);

	std::vector<glm::vec2> GetLandTiles() const;

	Tile* GetTileFromPos(glm::vec2 pos);
	glm::vec2 GetWorldPosFromTile(Tile* tile) const;
	glm::vec2 GridToWorldPos(glm::ivec2 pos) const;
	SDL_Rect GetRenderRect(glm::vec2 worldPos, glm::ivec2 size);

	bool IsInGrid(glm::ivec2 point, glm::ivec2 dir);

	Tile* SmallestEntropy();

	glm::ivec2 tileSizeOnScreen;
	std::vector<std::vector<Tile>> Tiles;

	std::vector<glm::ivec2> waterPositions{};

private:
	std::array<char, numberOfTypes> allTypes;
};

