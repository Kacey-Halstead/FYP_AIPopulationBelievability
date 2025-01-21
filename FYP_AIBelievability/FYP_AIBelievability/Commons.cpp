#include "Commons.h"
#include <SDL.h>

bool ComparePositions(glm::vec2 a, glm::vec2 b)
{
	glm::vec2 diff = b - a;
	float mag = glm::length(diff);
	return mag < 5.0f;
}
