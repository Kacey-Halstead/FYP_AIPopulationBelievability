#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <queue>

#include "TextureManager.h"
#include "ImGuiImplementation.h"
#include "PersonalityComponent.h"
#include "AStar.h"
#include "States.h"

struct States
{
	MoveToState moveState{};
	FindFoodState foodState{};
	FindState findState{};
	FindWaterState waterState{};
};

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

	void DetectFood(bool detect, glm::vec2 pos);
	void DetectWater(bool detect, glm::vec2 pos);

	void DrinkWater(float amount);

	int DecideOnGoal();

	States& GetStates() { return states; }

	inline Grid* GetGridRef() { return gridRef; }

	glm::vec2 position = {};

	Needs needs = {};

	PersonalityComponent personalityComponent;

	int agentCount = 0;
	SDL_Point size = { 50, 50 };

private:
	States states;

	SDL_Rect detectRect;
	SDL_Rect agentRect;	

	std::array<Agent*, 2> parents = {};

	Grid* gridRef;

	std::vector<glm::ivec2> patrolPositions = { {1, 1}, {gridSizeX-1, 1}, {1, gridSizeY - 1}, {25, 25}, {(gridSizeX - 1 )/2, (gridSizeY - 1 )/2} };


	glm::vec2 velocity = { 0, 0 };
	float speed = 100.0f;

};



