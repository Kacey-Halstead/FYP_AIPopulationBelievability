#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "TextureManager.h"
#include "ImGuiImplementation.h"
#include "PersonalityComponent.h"
#include "GOAP.h"
#include "Commons.h"

struct state
{
	Tile* to;
	Tile* from;
	Agent* agent;
	std::vector<Node> path;
};

class Agent
{
public:
	Agent();
	Agent(Agent* P1, Agent* P2);
	~Agent();
	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer, SDL_Window* window);
	bool IsPointInAgent(SDL_Point point);
	void DecreaseNeeds(float deltaTime);
	bool Move(SDL_Point destination);

	SDL_Point position = {};

	GOAP GOAPComponent;
	PersonalityComponent personalityComponent;
	int agentCount = 0;
	Needs needs = {};
	SDL_Point velocity = {};

private:
	SDL_Point size = {50, 50};
	SDL_Rect agentRect;	
	std::array<Agent*, 2> parents = {};
};
