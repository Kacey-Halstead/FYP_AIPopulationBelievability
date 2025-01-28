#include "Commons.h"
#include <SDL.h>

bool ComparePositions(glm::ivec2 a, glm::ivec2 b, int acceptanceRadius)
{
	glm::ivec2 diff = b - a;
	return diff.x < acceptanceRadius && diff.y < acceptanceRadius;
}
