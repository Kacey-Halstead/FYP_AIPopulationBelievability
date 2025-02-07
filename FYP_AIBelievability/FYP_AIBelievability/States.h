#pragma once
#include <glm/glm.hpp>
#include <list>
#include "AStar.h"
#include "FoodSource.h"

class Agent;

struct MoveToState
{
	glm::vec2 to = {1, 1};
	glm::vec2 from;

	bool isMoveToSet = false;

	std::vector<Node> path;

	glm::vec2 nextInPatrol;

	int patrolIndex = 0;
};

struct FindState
{
	std::array<glm::vec2, 5> patrolPoints;  //vec of pairs: pos and bool (if checked)

	glm::vec2 nextToCheck;

	bool isFound = false;
};

struct FindFoodState
{
	std::list<std::pair<glm::vec2, FoodSource*>> prevFoodPositions;

	FoodSource* foundFoodRef = nullptr;

	bool complete = false;
};

struct FindWaterState
{
	std::vector<glm::vec2> prevWaterPositions;

	glm::vec2 foundWaterRef = {-1, -1};

	bool waterRefSet = false;

	bool complete = false;
};

struct SocialiseState
{
	std::vector<Agent*> otherAgents;

	Agent* agentRef = nullptr;

	//is agent being spoken to?
	bool isTalkingTo = false;

	glm::vec2 runAwayPosBefore;

	//is agent currently seeking another agent? (so agents cannot interrupt eachother)
	bool isSeekingOtherAgent = false;
};