#include "FoodSource.h"

FoodSource::FoodSource()
{

}

FoodSource::FoodSource(Grid* grid)
{
	foodAmount = maxfoodAmount;
	gridRef = grid;

	std::uniform_int_distribution<> distrib(1, gridSizeX-1);

	float triedPosX;
	float triedPosY;
	while (true)
	{
		triedPosX = distrib(RandomGenerator::gen);
		triedPosY = distrib(RandomGenerator::gen);

		if (gridRef->GetTileFromPos(glm::vec2(triedPosX, triedPosY))->GetType() == 'L') // if type land
		{
			break;
		}
	}
	position.x = triedPosX * grid->tileSize.x;
	position.y = triedPosY * grid->tileSize.y;

	rect = { (int)position.x, (int)position.y, size.x, size.y };
	detectRect = { rect.x - ((rect.w * 5)/2), rect.y - ((rect.h * 5)/2), rect.w * 5, rect.h * 5};
}

void FoodSource::Render(SDL_Renderer* renderer, SDL_Window* window)
{
	canEat ? indexForTexture = BUSHF : indexForTexture = BUSHE;
	SDL_RenderCopy(renderer, TextureManager::GetTexture(indexForTexture), NULL, &rect);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderDrawRect(renderer, &detectRect);
}

void FoodSource::Update(float deltaTime)
{
	Replenish(deltaTime);
	foodAmount > 0 ? canEat = true : canEat = false;
;}

void FoodSource::EatFrom(float amount)
{
	if (canEat)
	{
		foodAmount -= amount;
	}
}

void FoodSource::Replenish(float deltaTime)
{
	if (foodAmount < maxfoodAmount)
	{
		counter += deltaTime * 2;

		//only replishes every timestamp
		if (counter > 5)
		{
			counter = 0;
			foodAmount++;
		}
	}
}

bool FoodSource::isInRect(glm::vec2 point)
{
	SDL_Point pos = { point.x, point.y };
	return SDL_PointInRect(&pos, &detectRect);
}
