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
	bool EatFrom(float amount);
	void Replenish(float deltaTime);
	bool isInRect(glm::vec2 point);

	bool canEat = true;
	bool taken = true;
	glm::vec2 position;

private:
	SDL_Rect rect;
	SDL_Rect detectRect;
	float foodAmount;

	TextureIndexes indexForTexture = BUSHF; 
	glm::ivec2 size = { 30, 30 };

	Grid* gridRef;

	const float maxfoodAmount = 100;



	//for replenishment
	int counter = 0;
};

