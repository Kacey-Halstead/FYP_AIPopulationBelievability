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

	std::uniform_int_distribution<> distrib(0, gridRef->landTilePositions.size()-1);

	// need to make sure can't gen on same tile
	position = gridRef->landTilePositions[distrib(RandomGenerator::gen)];

	gridRef->landTilePositions.erase(gridRef->landTilePositions.begin() + (distrib(RandomGenerator::gen) -1));
}

void FoodSource::Render(SDL_Renderer* renderer, SDL_Window* window) const
{
	TextureIndexes indexForTexture = BUSHE;
	if (canEat)
	{
		indexForTexture = isBlue ? BLUE_BUSH : BUSHF;
	}
	
	SDL_Rect rect = gridRef->GetRenderRect(position, size);
	SDL_RenderCopy(renderer, TextureManager::GetTexture(indexForTexture), NULL, &rect);
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