#pragma once
#include <vector>
#include <array>
#include <SDL_rect.h>
#include "Commons.h"

using namespace std;

class Tile
{
public:

	Tile();
	Tile(glm::ivec2 gPos, int intIndex);

	void Reset();
	void UpdateTypeandWeight(char c, float weightChange);

	inline char GetType() { return type; };
	inline void SetType(char toSet) { type = toSet; };

	inline glm::ivec2 GetGridPos() { return gridPos; };
	inline glm::vec2 GetWorldPos() { return worldPos; };

	inline void SetWorldPos(float toSetX, float toSetY) { worldPos = glm::vec2(toSetX, toSetY); };

	std::vector<std::pair<char, float>> typesAndWeights;
	std::array<char, numberOfTypes> availableTypes; //currently available types

	bool walkable = true;
	bool isInPath = false;

	int index = 0;

private:
	char type = '0';

	glm::vec2 worldPos;
	glm::ivec2 gridPos;
};




