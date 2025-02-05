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
	FoodSource(Grid* grid, bool isBlueBush);
	void Render(SDL_Renderer* renderer, SDL_Window* window) const;

	void Update(float deltaTime);
	bool EatFrom();
	void Replenish(float deltaTime);

	bool canEat = true;
	glm::vec2 position;

	bool isBlue = false;

private:
	SDL_Rect rect;
	float foodAmount;

	glm::ivec2 size = { 30, 30 };

	Grid* gridRef;

	float maxfoodAmount = 100;

	//for replenishment
	int counter = 0;
};

