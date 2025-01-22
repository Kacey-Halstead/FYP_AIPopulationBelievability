#pragma once
#include <glm/glm.hpp>
#include "AStar.h"
#include "FoodSource.h"

class Agent;

struct state
{

};

struct MoveToState : public state
{
	glm::vec2 to = {1, 1};
	glm::vec2 from;

	Agent* agent = nullptr;

	bool isMoveToSet = false;

	std::vector<Node> path;

	glm::vec2 nextInPatrol;

	bool IsComplete();
};

struct FindFoodState : public state
{
	std::vector<glm::vec2> prevFoodPositions;

	std::array<std::pair<glm::vec2, bool>, 5> patrolPoints;  //vec of pairs: pos and bool (if checked)

	glm::vec2 nextToCheck;

	bool isFoodFound = false;

	FoodSource* foundFoodRef = nullptr;

	bool eaten;

	bool IsComplete();
};