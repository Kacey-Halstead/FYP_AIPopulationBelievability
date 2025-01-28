#pragma once
#include <vector>
#include <array>
#include <SDL_rect.h>
#include "Commons.h"

using namespace std;

class Tile
{
public:
	Tile(glm::ivec2 gPos, glm::ivec2 tileSize, int intIndex);

	void Reset();
	void UpdateTypeandWeight(char c, float weightChange);

	inline char GetType() const { return type; };
	inline void SetType(char toSet) { type = toSet; };

	inline glm::ivec2 GetGridPos() const { return gridPos; };

	std::vector<std::pair<char, float>> typesAndWeights;
	std::array<char, numberOfTypes> availableTypes; //currently available types

	bool walkable = true;
	bool isInPath = false;

	int index = 0;

private:
	char type = '0';

	glm::ivec2 gridPos;
};




