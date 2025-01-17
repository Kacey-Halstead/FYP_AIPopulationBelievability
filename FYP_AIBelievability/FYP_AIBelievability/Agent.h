#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>

#include "TextureManager.h"
#include "ImGuiImplementation.h"
#include "PersonalityComponent.h"
#include "AStar.h"

class Agent;

struct MoveToState
{
	glm::vec2 to;
	glm::vec2 from;
	Agent* agent = nullptr;
	std::vector<Node> path;

	bool IsComplete();
};

class Agent
{
public:
	Agent();
	Agent(Grid* grid, Agent* P1, Agent* P2);

	~Agent();
	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer, SDL_Window* window);
	bool IsPointInAgent(SDL_Point point);
	void DecreaseNeeds(float deltaTime);
	void Move(glm::vec2 destination);
	Tile* GetTileFromPos(glm::vec2 pos);

	void UpdateImGui();

	glm::vec2 position = {};

	PersonalityComponent personalityComponent;
	int agentCount = 0;
	Needs needs = {};
	glm::vec2 velocity = {0, 0};
	Grid* gridRef;
	float speed = 100.0f;

	MoveToState& GetState() { return moveState; }

private:
	MoveToState moveState;
	SDL_Point size = {50, 50};
	SDL_Rect agentRect;	
	std::array<Agent*, 2> parents = {};


};



