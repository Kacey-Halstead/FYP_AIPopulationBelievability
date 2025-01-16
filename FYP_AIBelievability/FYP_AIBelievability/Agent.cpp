#include "Agent.h"

Agent::Agent()
{
}

Agent::Agent(Grid* grid)
{
	agentCount = ImGui_Implementation::agentCount;
	moveState.agent = this;

	gridRef = grid;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { (int)position.x, (int)position.y, size.x, size.y };

	//generates random personality
	personalityComponent = PersonalityComponent();

	//Sets GOAP Component
	GOAPComponent = GOAP();


	//sets extern variables
	ImGui_Implementation::OCEANValues = personalityComponent.OCEANValues;
	ImGui_Implementation::Traits = personalityComponent.traits;
	ImGui_Implementation::needStruct = needs;
}

Agent::Agent(Agent* P1, Agent* P2)
{
	agentCount = ImGui_Implementation::agentCount;

	gridRef = P1->gridRef;

	position.x = rand() % 200 + 1;
	position.y = rand() % 200 + 1;
	agentRect = { (int)position.x, (int)position.y, size.x, size.y };

	//generates personality from parents
	personalityComponent = PersonalityComponent(P1, P2);
	parents[0] = P1;
	parents[0] = P2;

	//Sets GOAP Component
	GOAPComponent = GOAP();

	//sets extern variables
	ImGui_Implementation::OCEANValues = personalityComponent.OCEANValues;
	ImGui_Implementation::Traits = personalityComponent.traits;
	ImGui_Implementation::needStruct = needs;
}

Agent::~Agent()
{
	
}

void Agent::Update(float deltaTime)
{
	agentRect.x = position.x - agentRect.w/2;
	agentRect.y = position.y - agentRect.h/2;

	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
	velocity = { 0.0f, 0.0f };

	DecreaseNeeds(deltaTime);
}

void Agent::Render(SDL_Renderer* renderer, SDL_Window* window)
{
	SDL_RenderCopy(renderer, TextureManager::GetTexture(AGENT), NULL, &agentRect);
	SDL_RenderDrawRect(renderer, &agentRect);
}

bool Agent::IsPointInAgent(SDL_Point point)
{
	return SDL_PointInRect(&point, &agentRect);
}

void Agent::DecreaseNeeds(float deltaTime)
{
	if (needs.hungerVal > 0)
	{
		needs.hungerVal -= 1 * deltaTime;
	}

	if (needs.thirstVal > 0)
	{
		needs.thirstVal -= 5 * deltaTime;
	}
}

void Agent::Move(SDL_FPoint destination)
{
	SDL_FPoint pos = destination - position;
	velocity = Normalize(pos);
	velocity = { velocity.x * speed, velocity.y * speed };
}

Tile* Agent::GetTileFromPos(SDL_Point pos)
{
	SDL_Point tilePos = { round(pos.x/gridRef->tileSize.x), round(pos.y / gridRef->tileSize.y) };

	return gridRef->Tiles[tilePos.x][tilePos.y];
}

bool Agent::ComparePositions(SDL_FPoint a, SDL_FPoint b)
{
	SDL_FPoint diff = b - a;
	float mag = Magnitude(diff);
	return mag < 2.0f;
}

float Agent::Magnitude(SDL_FPoint a)
{
	return sqrt((a.x * a.x) + (a.y * a.y));
}

SDL_FPoint Agent::ToFPoint(SDL_Point a)
{
	SDL_FPoint convert = { a.x, a.y };
	return convert;
}

SDL_FPoint Agent::Normalize(SDL_FPoint a)
{
	float mag = Magnitude(a);
	SDL_FPoint norm = { a.x / mag, a.y / mag };
	return norm;
}

bool operator != (const SDL_FPoint& a, const SDL_FPoint& b)
{
	return ((a.x != b.x) && (a.y != b.y));
}

SDL_FPoint operator - (const SDL_FPoint& a, const SDL_FPoint& b)
{
	return { a.x - b.x, a.y - b.y };
}

bool MoveToState::IsComplete()
{
	if (agent->ComparePositions(agent->position, to))
	{
		return true;
	}

	return false;
}
