#include "Grid.h"
#include "Tile.h"

Grid::Grid(std::array<char, numberOfTypes> types)
{
	allTypes = types;
	tileSizeOnScreen = { 900 / gridSizeX, 900 / gridSizeY };

	//for every cell, define possible tiles
	int index = 0;
	Tiles.reserve(gridSizeX);
	for (int x = 0; x < gridSizeX; x++)
	{
		std::vector<Tile> newTiles;
		newTiles.reserve(gridSizeY);
		for (int y = 0; y < gridSizeY; y++)
		{
			newTiles.emplace_back(glm::ivec2(x, y),tileSizeOnScreen, index);
			index++;
		}
		Tiles.push_back(newTiles);
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
	return (0 <= pos.x && pos.x <= gridSizeX-1 
		&& 0 <= pos.y && pos.y <= gridSizeY-1);
}

bool Grid::IsInGrid(glm::ivec2 point)
{
	//if pos within X and Y bounds of grid
	return (0 <= point.x && point.x <= gridSizeX - 1
		&& 0 <= point.y && point.y <= gridSizeY - 1);
}

void Grid::RenderGrid(SDL_Renderer* renderer)
{
	int counter = 0;
	int index = 0;
	TextureIndexes spriteSheetIndex = WORLD;
	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			SDL_Rect destRect{ x * tileSizeOnScreen.x, y * tileSizeOnScreen.y, tileSizeOnScreen.x, tileSizeOnScreen.y };
			SDL_Rect sourceRect{};
			index = (y * gridSizeX) + x;

			switch (Tiles[x][y].GetType())
			{
			case 'S':
				spriteSheetIndex = WATER_SAND;
				sourceRect = { sourceRectPositions[index].x * 16, sourceRectPositions[index].y * 16 , 16, 16 };
				break;
			case 'L':
				spriteSheetIndex = WORLD;
				sourceRect = { 16 * 16, 12 * 16, 16, 16 };
				break;
			case 'C':
				spriteSheetIndex = LAND_OVERLAP;
				sourceRect = { sourceRectPositions[index].x * 16, sourceRectPositions[index].y * 16 , 16, 16 };
				break;
			default:
				break;
			}

			SDL_RenderCopy(renderer, TextureManager::GetTexture(spriteSheetIndex), &sourceRect, &destRect);

			counter++;

			//CORNERS
			if (!sourceRectPositionsCorners[index].empty())
			{
				for (const auto& corners : sourceRectPositionsCorners[index])
				{
					SDL_Rect cornerSourceRect = { 4 * 16, 0, 16, 16 };
					SDL_Rect cornerDestRect = { destRect.x, destRect.y, tileSizeOnScreen.x, tileSizeOnScreen.y };
					SDL_RendererFlip flipMode = SDL_FLIP_NONE;
					switch (corners.first)
					{
					case 0:
						flipMode = SDL_FLIP_HORIZONTAL;
						break;
					case 1:
						cornerSourceRect = { 4 * 16, 1 * 16, 16, 16 };
						break;
					case 2:
						flipMode = SDL_FLIP_VERTICAL;
						break;
					}

					SDL_RenderCopyEx(renderer, TextureManager::GetTexture(corners.second), &cornerSourceRect, &cornerDestRect, 0.0f, NULL, flipMode);
					
				}
			}

			//DECOR
			if (decorSourceRectPositions[index] != glm::ivec2(0, 0)) //if decor in space
			{
				SDL_Rect decorSourceRect = { decorSourceRectPositions[index].x * 16, decorSourceRectPositions[index].y * 16, 16, 16 };
				SDL_Rect decorDestRect = { x * tileSizeOnScreen.x, y * tileSizeOnScreen.y, tileSizeOnScreen.x, tileSizeOnScreen.y };

				SDL_RenderCopy(renderer, TextureManager::GetTexture(WORLD), &decorSourceRect, &decorDestRect);
			}
		}
	}
}

glm::ivec2 Grid::GetClosestTileOfType(char type, glm::vec2 position)
{
	std::vector<glm::ivec2> allTilesofType = GetTilesOfType(type);
	std::pair<float, glm::ivec2> closest = std::make_pair(std::numeric_limits<float>().max(), position); //distance + position

	for (const auto& tile : allTilesofType)
	{
		float distance = DistanceBetween(position, tile);
		if (distance < closest.first)
		{
			closest = std::make_pair(distance, tile);
		}
	}

	return closest.second;
}

glm::ivec2 Grid::GetClosestTileOfType(std::vector<char> types, glm::vec2 position)
{
	std::vector<glm::ivec2> closestTiles{};

	for (const auto& type : types)
	{
		closestTiles.push_back(GetClosestTileOfType(type, position));
	}

	std::pair<float, glm::ivec2> closestDistance = std::make_pair(std::numeric_limits<float>().max(), glm::ivec2(0, 0));

	for (const auto& tile : closestTiles)
	{
		float distanceBetween = DistanceBetween(position, tile);
		if (distanceBetween < closestDistance.first)
		{
			closestDistance.first = distanceBetween;
			closestDistance.second = tile;
		}
	}


	return closestDistance.second;
}

std::vector<glm::ivec2> Grid::GetTilesOfType(char type)
{
	std::vector<glm::ivec2> landTileWorldPositions;
	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			if (Tiles[x][y].GetType() == type)
			{
				landTileWorldPositions.emplace_back(Tiles[x][y].GetGridPos());
			}
		}
	}
	return landTileWorldPositions;
}

Tile* Grid::GetTileFromPos(glm::vec2 pos)
{
	glm::vec2 tilePos = { floor(pos.x), floor(pos.y) };

	if (tilePos.y > gridSizeY - 1) tilePos.y = gridSizeY - 1;
	if (tilePos.x > gridSizeX - 1) tilePos.x = gridSizeX - 1;
	if (tilePos.y < 0) tilePos.y = 0;
	if (tilePos.x < 0) tilePos.x = 0;

	return &Tiles[tilePos.x][tilePos.y];
}

glm::vec2 Grid::GridToWorldPos(glm::ivec2 pos) const
{
	return glm::vec2{ pos.x + .5f, pos.y +.5f };
}

SDL_Rect Grid::GetRenderRect(glm::vec2 worldPos, glm::ivec2 size)
{
	glm::ivec2 screenPos = worldPos * glm::vec2(tileSizeOnScreen);

	SDL_Rect rect{
		screenPos.x - size.x/2, screenPos.y - size.y/2,
		size.x, size.y
	};
	return rect;
}

glm::vec2 Grid::GetWorldPosFromTile(Tile* tile) const
{
	glm::vec2 pos = {tile->GetGridPos().x * tileSizeOnScreen.x, tile->GetGridPos().y * tileSizeOnScreen.y};
	return pos;
}
