#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "TextureManager.h"
#include "ImGuiImplementation.h"
#include "PersonalityComponent.h"
#include "GOAP.h"
#include "Commons.h"


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

	SDL_Point position = {};

	GOAP GOAPComponent;
	PersonalityComponent personalityComponent;
	int agentCount = 0;
	Needs needs = {};

private:
	SDL_Point size = {50, 50};
	SDL_Rect agentRect;	
	std::array<Agent*, 2> parents = {};
};
