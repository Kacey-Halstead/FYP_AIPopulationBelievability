#include "FoodSource.h"
#include "Tile.h"

FoodSource::FoodSource()
{

}

FoodSource::FoodSource(Grid* grid)
{
	foodAmount = maxfoodAmount;
	gridRef = grid;

	auto landTiles = gridRef->GetLandTiles();
	std::uniform_int_distribution<> distrib(0, landTiles.size()-1);

	// need to make sure can't gen on same tile
	position = landTiles[distrib(RandomGenerator::gen)];
}

void FoodSource::Render(SDL_Renderer* renderer, SDL_Window* window) const
{
	TextureIndexes indexForTexture = canEat ?  BUSHF : BUSHE;
	SDL_Rect rect = gridRef->GetRenderRect(position, size);
	SDL_RenderCopy(renderer, TextureManager::GetTexture(indexForTexture), NULL, &rect);
	SDL_RenderDrawRect(renderer, &rect);
}

void FoodSource::Update(float deltaTime)
{
	Replenish(deltaTime);
	foodAmount > 10 ? canEat = true : canEat = false;
;}

bool FoodSource::EatFrom(float amount)
{
	if (canEat)
	{
		foodAmount -= amount;
		return true;
	}
	return false;
}

void FoodSource::Replenish(float deltaTime)
{
	if (foodAmount < maxfoodAmount)
	{
		counter += deltaTime * 10;

		//only replishes every timestamp
		if (counter > 5)
		{
			counter = 0;
			foodAmount++;
		}
	}
}