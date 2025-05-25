#include "FoodSource.h"
#include "Tile.h"

FoodSource::FoodSource()
{

}

FoodSource::FoodSource(Grid* grid, bool isBlueBush)
{
	foodAmount = maxfoodAmount;
	gridRef = grid;

	if (isBlueBush) isBlue = true;

	if (!grid->landTilePositions.empty())
	{
		std::uniform_int_distribution<> distrib(0, gridRef->landTilePositions.size() - 1);

		// need to make sure can't gen on same tile
		int index = distrib(RandomGenerator::gen);
		position = gridRef->landTilePositions[index];

		gridRef->landTilePositions.erase(gridRef->landTilePositions.begin() + (distrib(RandomGenerator::gen) - 1));
	}
}

void FoodSource::Render(SDL_Renderer* renderer, SDL_Window* window) const
{

	//int spriteYPos = 1;
	//int spriteXPos = 3;
	//TextureIndexes indexForTexture = isBlue ? BUSHES_BLUE : BUSHES_RED;

	//if (!canEat)
	//{

	//	spriteYPos = 0;
	//	spriteXPos = 4;
	//}

	//SDL_Rect sourceRect{ spriteXPos * 16, spriteYPos * 16, 16, 16};
	//
	//SDL_Rect rect = gridRef->GetRenderRect(position, size);
	//SDL_RenderCopy(renderer, TextureManager::GetTexture(indexForTexture), &sourceRect, &rect);
}

void FoodSource::Update(float deltaTime)
{
	Replenish(deltaTime);
	foodAmount > 20 ? canEat = true : canEat = false;
}

bool FoodSource::EatFrom()
{
	if (canEat)
	{
		foodAmount = 0;
		return true;
	}
	return false;
}

void FoodSource::Replenish(float deltaTime)
{
	counter += deltaTime * 1000;

	//only replishes every timestamp
	if (counter > 1 && foodAmount < maxfoodAmount)
	{
		foodAmount += 0.01;
		counter = 0;
	}
}