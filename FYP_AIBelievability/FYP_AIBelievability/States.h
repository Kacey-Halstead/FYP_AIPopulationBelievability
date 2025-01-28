#pragma once
#include <glm/glm.hpp>
#include "AStar.h"
#include "FoodSource.h"

class Agent;

struct MoveToState
{
	glm::ivec2 to = {1, 1};
	glm::ivec2 from;

	Agent* agent = nullptr;

	bool isMoveToSet = false;

	std::vector<Node> path;

	glm::ivec2 nextInPatrol;

	int patrolIndex = 0;

	bool IsComplete();
};

struct FindState
{
	std::array<glm::ivec2, 5> patrolPoints;  //vec of pairs: pos and bool (if checked)

	glm::ivec2 nextToCheck;

	bool isFound = false;

	bool IsComplete();
};

struct FindFoodState
{
	std::vector<glm::ivec2> prevFoodPositions;

	FoodSource* foundFoodRef = nullptr;

	bool complete = false;

	bool IsComplete();
};

struct FindWaterState
{
	std::vector<glm::ivec2> prevWaterPositions;

	glm::ivec2 foundWaterRef = {-1, -1};

	bool waterRefSet = false;

	bool complete = false;

	bool IsComplete();
};