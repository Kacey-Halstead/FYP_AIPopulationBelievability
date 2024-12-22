#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <random>
#include <time.h>
#include "TextureManager.h"
#include "ImGuiImplementation.h"

class Agent
{
public:
	Agent();
	~Agent();
	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer, SDL_Window* window);

	bool IsPointInAgent(SDL_Point point);

	SDL_Point position = { 10, 10 };

	std::array<float, 5> OCEANVals = {0, 0, 0, 0, 0};
	std::array<std::string, 6> traits;
	std::array<Agent*, 2> parents = {nullptr, nullptr};
	int agentCount;

private:
	SDL_Point size = {50, 50};
	SDL_Rect agentRect;

	void CalculateOCEANVals();	
	void GenerateTraits();
	
};
