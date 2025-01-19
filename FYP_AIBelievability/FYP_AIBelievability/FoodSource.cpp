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
}

void FoodSource::Render(SDL_Renderer* renderer, SDL_Window* window)
{
	canEat ? indexForTexture = BUSHF : indexForTexture = BUSHE;
	SDL_RenderCopy(renderer, TextureManager::GetTexture(indexForTexture), NULL, &rect);
	SDL_RenderDrawRect(renderer, &rect);
}

void FoodSource::Update(float deltaTime)
{
	Replenish(deltaTime);
;}

void FoodSource::EatFrom(float amount)
{
	foodAmount > 0 ? canEat = true : canEat = false;

	if (canEat)
	{
		foodAmount -= amount;
	}
}

void FoodSource::Replenish(float deltaTime)
{
	if (foodAmount <= maxfoodAmount)
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
