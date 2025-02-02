#include "Agent.h"

Agent::Agent()
{

}

Agent::Agent(Grid* grid, Agent* P1, Agent* P2)
{
	agentCount = ImGui_Implementation::agentCount;
	states.moveState.agent = this;

	if (P1 == nullptr) //if no parents
	{
		//generates random personality
		personalityComponent = PersonalityComponent();

		//random positions
		position.x = 30 * ((float)rand() / RAND_MAX);
		position.y = 30* ((float)rand() / RAND_MAX);
	}
	else
	{
		//generates personality from parents
		personalityComponent = PersonalityComponent(P1, P2);
		parents[0] = P1;
		parents[1] = P2;
	}

	gridRef = grid;

	//sets extern variables
	ImGui_Implementation::OCEANValues = personalityComponent.OCEANValues;
	ImGui_Implementation::Traits = personalityComponent.traits;
	ImGui_Implementation::needStruct = needs;

	for (int i = 0; i < 5; i++)
	{
		states.findState.patrolPoints[i] = patrolPositions[i];
	}
}

Agent::~Agent()
{
	
}

void Agent::Update(float deltaTime)
{
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
	velocity = glm::vec2(0.0f);

	DecreaseNeeds(deltaTime);
	SettleEmotions(deltaTime);
}

void Agent::Render(SDL_Renderer* renderer, SDL_Window* window) const
{
	SDL_Rect destRect = gridRef->GetRenderRect(position, size);
	SDL_Rect emoteRect = {destRect.x, destRect.y + 10, 10, 10};
	SDL_SetTextureColorMod(TextureManager::GetTexture(AGENT), textureColour[0], textureColour[1], textureColour[2]);
	SDL_RenderCopy(renderer, TextureManager::GetTexture(AGENT), NULL, &destRect);
	SDL_RenderCopy(renderer, TextureManager::GetTexture(EMOTE_HAPPYFACE), NULL, &emoteRect);
	SDL_RenderDrawRect(renderer, &destRect);
}

bool Agent::IsPointInAgent(SDL_Point point)
{
	return SDL_PointInRect(&point, &agentRect);
}

void Agent::DetectFood(std::pair<glm::vec2, FoodSource*> foodPair)
{
	auto it = std::find(states.foodState.prevFoodPositions.begin(), states.foodState.prevFoodPositions.end(), foodPair);
	if (it == states.foodState.prevFoodPositions.end())
	{
		states.foodState.prevFoodPositions.push_back(foodPair);
	}
}

void Agent::DetectWater(glm::vec2 pos)
{
	auto it = std::find(states.waterState.prevWaterPositions.begin(), states.waterState.prevWaterPositions.end(), pos);
	if (it == states.waterState.prevWaterPositions.end())
	{
		states.waterState.prevWaterPositions.push_back(pos);
	}
}

void Agent::DetectOtherAgents(Agent* agent)
{
	auto it = std::find(states.socialState.otherAgents.begin(), states.socialState.otherAgents.end(), agent);
	if (it == states.socialState.otherAgents.end())
	{
		states.socialState.otherAgents.push_back(agent);
	}
}

void Agent::DrinkWater(float amount)
{
	needs.thirstVal += amount;
	if (needs.thirstVal > 100.0f)
	{
		needs.thirstVal = 100.0f;
	}
}

std::vector<float> Agent::GetValuesForImGui(int index)
{
	switch (index)
	{
	case 0: return hungerValues;
	case 1: return thirstValues;
	case 2: return socialValues;
	}

	return std::vector<float>();
}

std::pair<char, EEmotions> Agent::DecideOnGoal()
{
	std::vector<std::pair<char, float>> utilities{};

	utilities.push_back(make_pair('H', GetUtility(needs.hungerVal)));
	utilities.push_back(make_pair('T', GetUtility(needs.thirstVal)));
	utilities.push_back(make_pair('S', GetUtility(needs.socialVal)));

	std::sort(utilities.begin(), utilities.end(), [](std::pair<char, float> a, std::pair<char, float> b) {
		return a.second > b.second;
		});
	
	std::pair<char, float> highestUrgency = utilities[0];

	EEmotions dominantEmotion = GetDominantEmotion().first;

	if (highestUrgency.first == 'S' && dominantEmotion == SADNESS) //if social lowest but sad, skip
	{
		highestUrgency = utilities[1];
	}

	//if no needs are very low
	if (highestUrgency.second < 0.4)
	{
		return make_pair('W', dominantEmotion);
	}

	return make_pair(highestUrgency.first, dominantEmotion);
}

std::pair<EEmotions, float> Agent::GetDominantEmotion()
{
	std::pair<EEmotions, float> highest;
	highest.second = 0;

	for (std::pair<EEmotions, float> emotion : emotions)
	{
		if (emotion.second > highest.second)
		{
			highest = emotion;
		}
	}

	switch (highest.first)
	{
	case SURPRISE:
		textureColour = { 104, 211, 232 };
		break;
	case ANTICIPATION:
		textureColour = { 232, 134, 81 };
		break;
	case DISGUST:
		textureColour = { 232, 95, 175 };
		break;
	case JOY:
		textureColour = { 232, 207, 46 };
		break;
	case ANGER:
		textureColour = { 232, 32, 32 };
		break;
	case FEAR:
		textureColour = { 27, 128, 28 };
		break;
	case TRUST:
		textureColour = { 141, 227, 104 };
		break;
	case SADNESS:
		textureColour = { 44, 44, 212 };
		break;
	case NONE:
		textureColour = { 255, 255, 255 };
		break;
	}

	return highest;
}

void Agent::ChangeEmotionValue(EEmotions emotion, float value)
{
	for (int i = 0; i < 8; i++)
	{
		if (emotion == emotions[i].first)
		{
			emotions[i].second += (value * personalityComponent.emotionMultipliers[i]);
			break;
		}
	}

	SetSpeed();
}

void Agent::ApplyNeedModifiers()
{
	//utility theory 0/1 - more urgent need = unhappy

	float hungerUtility = GetUtility(needs.hungerVal);
	float thirstUtility = GetUtility(needs.thirstVal);

	float hungerModifier = 2 * hungerUtility;
	float thirstModifier = 2 * thirstUtility;
}

void Agent::DecreaseNeeds(float deltaTime)
{
	if (needs.hungerVal > 0)
	{
		needs.hungerVal -= 2 * deltaTime;
	}

	if (needs.thirstVal > 0)
	{
		needs.thirstVal -= 2 * deltaTime;
	}

	if (needs.socialVal > 0 && GetDominantEmotion().first != SADNESS) //do not decrease social when sad
	{
		needs.socialVal -= 2 * deltaTime;
	}

	if (needs.healthVal < 100)
	{
		needs.healthVal += 1 * deltaTime;
	}
}

void Agent::SettleEmotions(float deltaTime)
{
	//overtime, emotions settle to neutral (0)

	for (std::pair<EEmotions, float> emotion : emotions)
	{
		if (emotion.second > 0)
		{
			emotion.second -= 0.1f * deltaTime;
		}
		else
		{
			emotion.second += 0.1f * deltaTime;
		}
	}
}

void Agent::SetSpeed()
{
	EEmotions dominant = GetDominantEmotion().first;

	if (dominant == ANGER || dominant == JOY || dominant == FEAR)
	{
		speed = 2.0f;
	}
	else if (dominant == SADNESS || dominant == ANTICIPATION)
	{
		speed = 0.5f;
	}
	else
	{
		speed = 1.0f;
	}
}

float Agent::GetUtility(float need)
{
	return sqrt((100 - need) / 100);
}

void Agent::Move(glm::vec2 destination)
{
	glm::vec2 toDest = destination - position;
	if (toDest == glm::vec2(0.0f))
		return;
	velocity = glm::normalize(toDest) * speed;
}

void Agent::UpdateImGui()
{
	if (ImGui_Implementation::time.size() >= 400)
	{
		ImGui_Implementation::time.erase(ImGui_Implementation::time.begin());
	}

	if (hungerValues.size() >= 400)
	{
		hungerValues.erase(hungerValues.begin());
		thirstValues.erase(thirstValues.begin());
		socialValues.erase(socialValues.begin());
	}

	hungerValues.push_back(needs.hungerVal);
	thirstValues.push_back(needs.thirstVal);
	socialValues.push_back(needs.socialVal);
}


