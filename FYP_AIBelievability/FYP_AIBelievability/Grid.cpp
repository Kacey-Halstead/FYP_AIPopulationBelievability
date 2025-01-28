#include "Grid.h"
#include "Tile.h"

Grid::Grid(std::array<char, numberOfTypes> types)
{
	allTypes = types;
	tileSize = { windowSize.x / gridSizeX, windowSize.y / gridSizeY };

	//for every cell, define possible tiles
	int index = 0;
	Tiles.reserve(gridSizeX);
	for (int x = 0; x < gridSizeX; x++)
	{
		std::vector<Tile> newTiles;
		newTiles.reserve(gridSizeY);
		for (int y = 0; y < gridSizeY; y++)
		{
			newTiles.emplace_back(glm::ivec2(x, y),tileSize, index);
			index++;
		}
		Tiles.push_back(newTiles);
	}

	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			SDL_Rect newRec{ x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y };
			rects.emplace_back(newRec);
		}
	}
}

Grid::~Grid()
{

}

Tile* Grid::SmallestEntropy()
{
	//find cell with smallest number of options
	std::vector<Tile*> smallestTiles;
	int numOptions = 3;

	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			if (Tiles[x][y].typesAndWeights.size() < numOptions && Tiles[x][y].GetType() == '0')
			{
				smallestTiles.clear();
				smallestTiles.push_back(&Tiles[x][y]);
				numOptions = Tiles[x][y].typesAndWeights.size();
			}
			else if (Tiles[x][y].typesAndWeights.size() == numOptions && Tiles[x][y].GetType() == '0')
			{
				smallestTiles.push_back(&Tiles[x][y]);
			}
		}
	}

	if (smallestTiles.size() == 1)
	{
		return smallestTiles[0];
	}
	else if(smallestTiles.size() > 1)
	{
		int random = rand() % (smallestTiles.size());
		return smallestTiles[random];
	}
	else
	{
		for (int x = 0; x < gridSizeX; x++)
		{
			for (int y = 0; y < gridSizeY; y++)
			{
				if (Tiles[x][y].GetType() == '0')
				{
					return &Tiles[x][y];
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
	return (0 <= pos.x && pos.x < gridSizeX 
		&& 0 <= pos.y && pos.y < gridSizeY);
}

void Grid::RenderGrid(SDL_Renderer* renderer)
{
	int counter = 0;
	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			if (Tiles[x][y].isInPath)
			{
				SDL_RenderCopy(renderer, TextureManager::GetTexture(PATH), NULL, &rects[counter]);
				//SDL_RenderDrawRect(renderer, &rects[counter]);
				counter++;
				continue;
			}

			switch (Tiles[x][y].GetType())
			{
			case 'S':
				SDL_RenderCopy(renderer, TextureManager::GetTexture(SEA), NULL, &rects[counter]);
				break;
			case 'L':
				SDL_RenderCopy(renderer, TextureManager::GetTexture(LAND), NULL, &rects[counter]);
				break;
			case 'C':
				SDL_RenderCopy(renderer, TextureManager::GetTexture(COAST), NULL, &rects[counter]);
				break;
			default:
				break;
			}
			//SDL_RenderDrawRect(renderer, &rects[counter]);
			counter++;
		}
	}
}

std::vector<glm::vec2> Grid::GetLandTiles() const
{
	std::vector<glm::vec2> landTileWorldPositions;
	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			if (Tiles[x][y].GetType() == 'L')
			{
				landTileWorldPositions.emplace_back(Tiles[x][y].GetGridPos());
			}
		}
	}
	return landTileWorldPositions;
}

Tile* Grid::GetTileFromPos(glm::vec2 pos)
{
	glm::vec2 tilePos = { floor(pos.x / tileSize.x), floor(pos.y / tileSize.y) };

	return &Tiles[tilePos.x][tilePos.y];
}

glm::vec2 Grid::GridToWorldPos(glm::ivec2 pos) const
{
	return glm::vec2{ pos.x * tileSize.x, pos.y * tileSize.y };
}

glm::vec2 Grid::GetWorldPosFromTile(Tile* tile) const
{
	glm::vec2 pos = {tile->GetGridPos().x * tileSize.x, tile->GetGridPos().y * tileSize.y};
	return pos;
}
