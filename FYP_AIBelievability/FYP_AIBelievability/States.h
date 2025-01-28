#pragma once
#include <glm/glm.hpp>
#include "AStar.h"
#include "FoodSource.h"

class Agent;

struct State
{

};

struct MoveToState : State
{
	glm::vec2 to = {1, 1};
	glm::vec2 from;

	Agent* agent = nullptr;

	bool isMoveToSet = false;

	std::vector<Node> path;

	glm::vec2 nextInPatrol;

	int patrolIndex = 0;

	bool IsComplete();
};

struct FindState : State
{
	std::array<glm::vec2, 5> patrolPoints;  //vec of pairs: pos and bool (if checked)

	glm::vec2 nextToCheck;

	bool isFound = false;

	bool IsComplete();
};

struct FindFoodState : State
{
	std::vector<std::pair<glm::vec2, FoodSource*>> prevFoodPositions;

	FoodSource* foundFoodRef = nullptr;

	bool complete = false;

	bool IsComplete();
};

struct FindWaterState : State
{
	std::vector<glm::vec2> prevWaterPositions;

	glm::vec2 foundWaterRef = {-1, -1};

	bool waterRefSet = false;

	bool complete = false;

	bool IsComplete();
};