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
	SocialiseState socialState{};
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
	void Move(glm::vec2 destination);
	void UpdateImGui();

	bool IsPointInAgent(SDL_Point point);

	void DetectFood(std::pair<glm::vec2, FoodSource*> foodPair);
	void DetectWater(glm::vec2 pos);
	void DetectOtherAgents(Agent* agent);

	void DrinkWater(float amount);

	char DecideOnGoal();

	inline Grid* GetGridRef() { return gridRef; }

	glm::vec2 position = {};

	Emotions emotions = {};
	Needs needs = {};
	States states = {};
	PersonalityComponent personalityComponent = {};

	int agentCount = 0;
	glm::ivec2 size = { 50, 50 };

private:

	SDL_Rect agentRect;	

	std::array<Agent*, 2> parents = {};

	Grid* gridRef;

	std::vector<glm::ivec2> patrolPositions = { {2, 2}, {gridSizeX-2, 2}, {2, gridSizeY - 2}, {25, 25}, {(gridSizeX - 2 )/2, (gridSizeY - 2 )/2} };

	glm::vec2 velocity = { 0, 0 };

	float speed = 1.0f;



};



