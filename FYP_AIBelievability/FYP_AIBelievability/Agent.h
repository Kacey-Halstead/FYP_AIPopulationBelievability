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
	void Render(SDL_Renderer* renderer, SDL_Window* window) const;
	void DecreaseNeeds(float deltaTime);
	void Move(glm::ivec2 destination);
	void UpdateImGui();

	bool IsPointInAgent(SDL_Point point);

	glm::vec2 ScreenPos();

	void DetectFood(glm::ivec2 pos);
	void DetectWater(glm::ivec2 pos);

	void DrinkWater(float amount);

	int DecideOnGoal();

	States& GetStates() { return states; }

	inline Grid* GetGridRef() { return gridRef; }

	glm::ivec2 position = {};

	Needs needs = {};

	PersonalityComponent personalityComponent;

	int agentCount = 0;
	SDL_Point size = { 50, 50 };

private:
	States states;

	SDL_Rect agentRect;	

	std::array<Agent*, 2> parents = {};

	Grid* gridRef;

	std::vector<glm::ivec2> patrolPositions = { {2, 2}, {gridSizeX-2, 2}, {2, gridSizeY - 2}, {gridSizeX-2, gridSizeY-2}, {(gridSizeX - 2 )/2, (gridSizeY - 2 )/2} };


	glm::vec2 velocity = { 0, 0 };
	int speed = 100;

};



