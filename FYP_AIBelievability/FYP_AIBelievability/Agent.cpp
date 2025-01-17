#include "Agent.h"

Agent::Agent()
{

}

Agent::Agent(Grid* grid, Agent* P1, Agent* P2)
{
	agentCount = ImGui_Implementation::agentCount;
	moveState.agent = this;

	if (P1 == nullptr) //if no parents
	{
		//generates random personality
		personalityComponent = PersonalityComponent();

		//random positions
		position.x = rand() % 200 + 1;
		position.y = rand() % 200 + 1;
		agentRect = { (int)position.x, (int)position.y, size.x, size.y };

		gridRef = grid;
	}
	else
	{
		gridRef = P1->gridRef;

		agentRect = { (int)P1->position.x, (int)P1->position.y, size.x, size.y };

		//generates personality from parents
		personalityComponent = PersonalityComponent(P1, P2);
		parents[0] = P1;
		parents[0] = P2;
	}

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

void Agent::Move(glm::vec2 destination)
{
	velocity = glm::normalize(destination - position) * speed;
}

Tile* Agent::GetTileFromPos(glm::vec2 pos)
{
	glm::vec2 tilePos = { round(pos.x/gridRef->tileSize.x), round(pos.y / gridRef->tileSize.y) };

	return gridRef->Tiles[tilePos.x][tilePos.y];
}

void Agent::UpdateImGui()
{
	if (ImGui_Implementation::time.size() >= 400)
	{
		ImGui_Implementation::time.erase(ImGui_Implementation::time.begin());
	}

	if (ImGui_Implementation::hungerValues.size() >= 400)
	{
		ImGui_Implementation::hungerValues.erase(ImGui_Implementation::hungerValues.begin());
		ImGui_Implementation::thirstValues.erase(ImGui_Implementation::thirstValues.begin());
	}

	ImGui_Implementation::hungerValues.push_back(needs.hungerVal);
	ImGui_Implementation::thirstValues.push_back(needs.thirstVal);
}

bool operator != (const SDL_FPoint& a, const SDL_FPoint& b)
{
	return ((a.x != b.x) && (a.y != b.y));
}

bool MoveToState::IsComplete()
{
	if (ComparePositions(agent->position, to))
	{
		return true;
	}

	return false;
}
