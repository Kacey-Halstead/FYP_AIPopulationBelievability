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

	rect = { (int)(position.x * gridRef->tileSize.x), (int)(position.y * gridRef->tileSize.y), size.x, size.y };
	detectRect = { rect.x - ((rect.w * 5)/2), rect.y - ((rect.h * 5)/2), rect.w * 5, rect.h * 5};
}

void FoodSource::Render(SDL_Renderer* renderer, SDL_Window* window) const
{
	TextureIndexes indexForTexture = canEat ?  BUSHF : BUSHE;
	SDL_RenderCopy(renderer, TextureManager::GetTexture(indexForTexture), NULL, &rect);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderDrawRect(renderer, &detectRect);
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

bool FoodSource::IsInRect(glm::vec2 point)
{
	SDL_Point pos = { point.x, point.y };
	return SDL_PointInRect(&pos, &detectRect);
}
