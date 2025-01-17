#pragma once
#include <vector>
#include <array>
#include <SDL_rect.h>
#include "Commons.h"

using namespace std;

class Tile
{
public:
	char type = '0';
	std::vector<std::pair<char, float>> typesAndWeights;
	std::array<char, numberOfTypes> availableTypes; //currently available types
	std::array<char, numberOfTypes> allAvTypes; //all available types possible
	glm::ivec2 pos;
	glm::vec2 worldPos;
	bool walkable = true;
	bool isInPath = false;
	int index = 0;

	Tile(std::array<char, numberOfTypes> allTypes);
	Tile();
	void SetType(char newType);
	void Print();
	void Reset();

	inline void UpdateTypeandWeight(char c, float weightChange)
	{
		auto it = std::find_if(typesAndWeights.begin(), typesAndWeights.end(), [c](const auto& p) {
			return p.first == c;
			});

		if (it != typesAndWeights.end())
		{
			typesAndWeights.at(it - typesAndWeights.begin()).second += weightChange;
		}
	}

	friend bool operator ==(const Tile a, const Tile b)
	{
		return a.pos == b.pos;
	}
};




