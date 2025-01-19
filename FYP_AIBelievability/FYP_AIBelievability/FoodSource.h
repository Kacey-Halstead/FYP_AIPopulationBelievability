#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>

#include "Commons.h"
#include "RandomGenerator.h"
#include "TextureManager.h"
#include "Grid.h"

class FoodSource
{
public:
	FoodSource();
	FoodSource(Grid* grid);
	void Render(SDL_Renderer* renderer, SDL_Window* window);

	void Update(float deltaTime);
	void EatFrom(float amount);
	void Replenish(float deltaTime);

	bool canEat = true;

private:
	SDL_Rect rect;
	float foodAmount;
	glm::vec2 position;
	TextureIndexes indexForTexture = BUSHF; 
	glm::ivec2 size = { 30, 30 };

	Grid* gridRef;

	const float maxfoodAmount = 100;

	//for replenishment
	int counter = 0;
};

