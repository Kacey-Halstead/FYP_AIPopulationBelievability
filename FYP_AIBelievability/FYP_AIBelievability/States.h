#pragma once
#include <glm/glm.hpp>
#include "AStar.h"
#include "FoodSource.h"

class Agent;

struct MoveToState
{
	glm::vec2 to;
	glm::vec2 from;

	Agent* agent = nullptr;

	bool isMoveToSet = false;

	std::vector<Node> path;

	bool IsComplete();
};

struct FindFoodState
{
	std::vector<glm::vec2> prevFoodPositions;

	std::array<std::pair<glm::vec2, bool>, 5> patrolPoints;  //vec of pairs: pos and bool (if checked)

	glm::vec2 nextToCheck;

	bool isFoodFound = false;

	FoodSource foundFoodRef;

	bool eaten;

	bool IsComplete();
};