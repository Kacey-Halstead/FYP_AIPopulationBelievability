#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "TextureManager.h"
#include "ImGuiImplementation.h"
#include "PersonalityComponent.h"
#include "GOAP.h"
#include "AStar.h"
#include "Commons.h"

struct MoveToState
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
	Agent(Grid* grid);
	Agent(Agent* P1, Agent* P2);
	~Agent();
	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer, SDL_Window* window);
	bool IsPointInAgent(SDL_Point point);
	void DecreaseNeeds(float deltaTime);
	void Move(SDL_FPoint destination);
	Tile* GetTileFromPos(SDL_Point pos);

	inline SDL_FPoint Normalize(SDL_FPoint a)
	{
		float mag = sqrt((a.x * a.x) + (a.y * a.y));
		SDL_FPoint norm = { a.x / mag, a.y / mag };
		return norm;
	}


	SDL_FPoint position = {};

	GOAP GOAPComponent;
	PersonalityComponent personalityComponent;
	int agentCount = 0;
	Needs needs = {};
	SDL_FPoint velocity = {0, 0};
	Grid* gridRef;

	MoveToState m;

private:
	SDL_Point size = {50, 50};
	SDL_Rect agentRect;	
	std::array<Agent*, 2> parents = {};
};

bool operator != (const SDL_FPoint& a, const SDL_FPoint& b);

SDL_FPoint operator - (const SDL_FPoint& a, const SDL_FPoint& b);