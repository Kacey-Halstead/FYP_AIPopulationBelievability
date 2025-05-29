#include "Commons.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <SDL.h>

bool ComparePositions(glm::vec2 a, glm::vec2 b, float acceptanceRadius)
{
	glm::vec2 diff = b - a;
	float mag2 = glm::length2(diff);
	return mag2 < (acceptanceRadius * acceptanceRadius);
}

float DistanceBetween(glm::vec2 a, glm::vec2 b)
{
	glm::vec2 diff = b - a;
	float mag = glm::length(diff);
	return mag;
}
