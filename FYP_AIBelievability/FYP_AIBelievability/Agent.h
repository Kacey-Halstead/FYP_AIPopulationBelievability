#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <queue>
#include <stack>

#include "TextureManager.h"
#include "ImGuiImplementation.h"
#include "PersonalityComponent.h"
#include "AStar.h"
#include "States.h"

enum actionIDs : unsigned int;

struct States
{
	TextureIndexes emoteToSet = NO_EMOTE;
	Agent* agent = nullptr;

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
	Agent(Grid* grid);
	~Agent();

	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer, SDL_Window* window) const;
	void UpdateImGui();

	void Move(glm::vec2 destination);
	std::pair<char, EEmotions> DecideOnGoal();
	bool IsPointInAgent(SDL_Point point);

	//DETECT
	void DetectFood(std::pair<glm::vec2, FoodSource*> foodPair);
	void DetectWater(glm::vec2 pos);
	void DetectOtherAgents(Agent* agent);

	//NEEDS
	void DrinkWater(float amount);
	void DecreaseNeeds(float deltaTime);
	void SettleEmotions(float deltaTime);
	void SetSpeed(float amount);
	float GetUtility(float need);

	//EMOTIONS
	std::pair<EEmotions, float> GetDominantEmotion(); //SADJAFTS
	void ChangeEmotionValue(EEmotions emotion, float value); //multipliers applied here
	void ApplyNeedModifiers();

	//GETTERS
	inline Grid* GetGridRef() { return gridRef; }
	std::vector<float> GetValuesForImGui(int index); //0 - hunger, 1- thirst, 2 - social
	Agent* GetClosestAgent();
	void Reset();

	glm::vec2 position = {};

	std::vector<std::pair<EEmotions, float>> emotions = baseEmotions();


	Needs needs = {};
	States states = {};
	PersonalityComponent personalityComponent = {};

	int agentCount = 0;
	bool blueBushPref = false;
	glm::ivec2 size = { 50, 50 };

	std::stack<actionIDs> responsiveStack{};

	std::list<std::string> actions{"Wander"};

	bool active = false;

private:
	SDL_Rect agentRect;
	SDL_Rect sourceRect;

	int animIndex = 0;
	float animCounter = 0.0f;

	Grid* gridRef;

	std::vector<glm::ivec2> patrolPositions = { {2, 2}, {gridSizeX-2, 2}, {2, gridSizeY - 2}, {25, 25}, {(gridSizeX - 2 )/2, (gridSizeY - 2 )/2} };

	glm::vec2 velocity = { 0, 0 };

	float speed = 1.0f;

	bool facingLeft = true;


	float emoteCounter = 0;
	std::array<Uint8, 3> textureColour = {255, 255, 255 };

	//Imgui values update
	std::vector<float> hungerValues = std::vector<float>(400, 100);
	std::vector<float> thirstValues = std::vector<float>(400, 100);
	std::vector<float> socialValues = std::vector<float>(400, 100);

	inline std::vector<std::pair<EEmotions, float>> baseEmotions()
	{
		return { make_pair(SURPRISE, 0),
			make_pair(ANTICIPATION, 0),
			make_pair(DISGUST, 0),
			make_pair(JOY, 0),
			make_pair(ANGER, 0),
			make_pair(FEAR, 0),
			make_pair(TRUST, 0),
			make_pair(SADNESS, 0)};
	};
};




