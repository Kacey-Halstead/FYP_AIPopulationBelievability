#include "Agent.h"
#include "RandomGenerator.h"

Agent::Agent()
{

}

Agent::Agent(Grid* grid)
{
	agentCount = ImGui_Implementation::agentCount;

	gridRef = grid;
}

Agent::~Agent()
{
	
}

void Agent::Update(float deltaTime)
{
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;

	int maxSprites = 0;
	int rectYPos = 1;

	if (velocity.x > 0.0f) //moving right
	{
		maxSprites = 4;
		facingLeft = false;
	}
	else if (velocity.x < 0.0f) //moving left
	{
		maxSprites = 4;
		facingLeft = true;
	}
	else if (velocity == glm::vec2(0.0f, 0.0f)) //not moving
	{
		rectYPos = 0;
		maxSprites = 8;
	}

	animCounter += deltaTime;

	if (animCounter > 0.2f)
	{
		animIndex = (animIndex + 1) % maxSprites;
		animCounter = 0;
	}

	sourceRect = { 4 + animIndex * 24, 6 + rectYPos * 24, 18, 18 };

	velocity = glm::vec2(0.0f);

	DecreaseNeeds(deltaTime);
	SettleEmotions(deltaTime);

	if (states.emoteToSet != NO_EMOTE)
	{
		emoteCounter += 2 * deltaTime;

		if (emoteCounter > 10)
		{
			emoteCounter = 0;
			states.emoteToSet = NO_EMOTE;
		}
	}
}

void Agent::Render(SDL_Renderer* renderer, SDL_Window* window) const
{
	SDL_Rect destRect = gridRef->GetRenderRect(position, size);

	SDL_SetTextureColorMod(TextureManager::GetTexture(AGENT_SPRITES), textureColour[0], textureColour[1], textureColour[2]);
	SDL_RenderCopyEx(renderer, TextureManager::GetTexture(AGENT_SPRITES), &sourceRect, &destRect, 0.0f, NULL, static_cast<SDL_RendererFlip>(facingLeft));

	if (states.emoteToSet != NO_EMOTE)
	{
		SDL_Rect emoteRect = { destRect.x + (destRect.w / 2), destRect.y - 15, 20, 20 };
		SDL_RenderCopy(renderer, TextureManager::GetTexture(states.emoteToSet), NULL, &emoteRect);
	}

	if (ImGui_Implementation::agentCount == agentCount)
	{
		SDL_RenderDrawRect(renderer, &destRect);
	}
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
		if (foodPair.second->isBlue)
		{
			states.foodState.prevFoodPositions.push_front(foodPair);
		}
		else
		{
			states.foodState.prevFoodPositions.push_back(foodPair);
		}
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
	needs.thirstVal = 100.0f;
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

Agent* Agent::GetClosestAgent()
{
	if (states.socialState.otherAgents.empty()) return nullptr;

	std::pair<float, Agent*> closestAgent = std::make_pair(std::numeric_limits<float>().max(), states.socialState.otherAgents[0]);

	for (Agent* agent : states.socialState.otherAgents)
	{
		float dist = DistanceBetween(states.agent->position, agent->position);

		if (dist < closestAgent.first)
		{
			closestAgent = std::make_pair(dist, agent);
		}
	}

	return closestAgent.second;
}

void Agent::Reset()
{
	needs = Needs{};
	emotions = baseEmotions();
	personalityComponent = PersonalityComponent();
	actions = { "Wander" };
	states = {};
	states.agent = this;
	textureColour = { 255, 255, 255 };
	std::uniform_real_distribution<> distrib(1.0f, 29.0f);
	position.x = distrib(RandomGenerator::gen);
	position.y = distrib(RandomGenerator::gen);
	active = true;
	states.socialState.isTalkingTo = false;
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

	if (highest.second <= 0.5)
	{
		highest = make_pair(NONE, 0);
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
		textureColour = { 14, 99, 25 };
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
			emotions[i].second += (value * personalityComponent.emotionMultipliers[i])/10;

			if (emotions[i].second > 1)
			{
				emotions[i].second = 1;
			}
			break;
		}
	}

	switch (emotion)
	{
	case SURPRISE:
		states.emoteToSet = EMOTE_EXCLAIM;
		break;
	case ANTICIPATION:
		states.emoteToSet = EMOTE_ANGER;
		break;
	case DISGUST:
		states.emoteToSet = EMOTE_SWIRL;
		break;
	case JOY:
		states.emoteToSet = EMOTE_HAPPYFACE;
		break;
	case ANGER:
		states.emoteToSet = EMOTE_ANGRYFACE;
		break;
	case FEAR:
		states.emoteToSet = EMOTE_FEAR;
		break;
	case TRUST:
		states.emoteToSet = EMOTE_HEART;
		break;
	case SADNESS:
		states.emoteToSet = EMOTE_SADFACE;
		break;
	case NONE:
		states.emoteToSet = NO_EMOTE;
		break;
	}

	GetDominantEmotion();
	SetSpeed(0.0f);
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
		needs.hungerVal -= 1 * deltaTime;
	}

	if (needs.thirstVal > 0)
	{
		needs.thirstVal -= 1 * deltaTime;
	}

	if (needs.socialVal > 0 && GetDominantEmotion().first != SADNESS) //do not decrease social when sad
	{
		needs.socialVal -= 1 * deltaTime;
	}

	if (needs.healthVal < 100)
	{
		needs.healthVal += 1 * deltaTime;
	}
}

void Agent::SettleEmotions(float deltaTime)
{
	//overtime, emotions settle to neutral (0)

	for (std::pair<EEmotions, float>& emotion : emotions)
	{
		if (emotion.second > 0)
		{
			emotion.second -= 0.005f * deltaTime;
		}
		else if(emotion.second < 1)
		{
			emotion.second += 0.005f * deltaTime;
		}
	}
}

void Agent::SetSpeed(float amount)
{
	if (amount != 0.0f)
	{
		speed = amount;
		return;
	}

	EEmotions dominant = GetDominantEmotion().first;

	if (dominant == ANGER || dominant == JOY || dominant == FEAR)
	{
		speed = 1.5f;
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


