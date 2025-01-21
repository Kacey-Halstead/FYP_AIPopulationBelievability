#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>

#include "TextureManager.h"
#include "ImGuiImplementation.h"
#include "PersonalityComponent.h"
#include "AStar.h"
#include "States.h"

class Agent
{
public:

	Agent();
	Agent(Grid* grid, Agent* P1, Agent* P2);

	~Agent();

	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer, SDL_Window* window);
	void DecreaseNeeds(float deltaTime);
	void Move(glm::vec2 destination);
	void UpdateImGui();

	bool IsPointInAgent(SDL_Point point);

	MoveToState& GetState() { return moveState; }
	FindFoodState& GetFoodState() { return foodState; }

	glm::vec2 position = {};

	Needs needs = {};

	PersonalityComponent personalityComponent;

	int agentCount = 0;
	SDL_Point size = { 50, 50 };

private:
	MoveToState moveState;
	FindFoodState foodState;

	SDL_Rect agentRect;	

	std::array<Agent*, 2> parents = {};

	Grid* gridRef;

	std::vector<glm::vec2> patrolPositions = { {1, 1}, {gridSizeX, 1}, {1, gridSizeY}, {gridSizeX, gridSizeY}, {gridSizeX/2, gridSizeY/2} };


	glm::vec2 velocity = { 0, 0 };
	float speed = 100.0f;
};



