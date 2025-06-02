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

	inline glm::ivec2 GetGridPos() const { return gridPos; };
	inline glm::vec2 GetWorldPos() const { return worldPos; };

	//WFC
	void Reset();
	void UpdateTypeandWeight(char c, float weightChange);

	inline char GetType() const { return type; };
	inline void SetType(char toSet) { type = toSet; };

	std::vector<std::pair<char, float>> typesAndWeights;
	std::array<char, numberOfTypes> availableTypes; 

	inline bool IsWalkable() { return walkable; }
	inline void SetWalkable(bool isWalkable) { walkable = isWalkable; }

private:
	bool walkable = true;

	char type = '0';

	int index = 0;

	glm::vec2 worldPos{};
	glm::ivec2 gridPos{};
};




