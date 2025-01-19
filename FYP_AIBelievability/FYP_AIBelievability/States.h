#pragma once
#include <glm/glm.hpp>
#include "AStar.h"

class Agent;
class FoodSource;

struct MoveToState
{
	glm::vec2 to;
	glm::vec2 from;

	Agent* agent = nullptr;

	std::vector<Node> path;

	bool IsComplete();
};

struct FindFoodState
{
	std::vector<glm::vec2> prevFoodPositions;

	Agent* agent = nullptr;

	glm::vec2 nextToCheck;

	bool isFoodFound = false;

	FoodSource foundFoodRef;

	bool IsComplete();
};