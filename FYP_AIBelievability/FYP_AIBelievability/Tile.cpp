#include "Tile.h"
#include "iostream"

Tile::Tile(glm::ivec2 gPos, glm::ivec2 tileSize, int intIndex)
{
	for (int i = 0; i < allTypes.size(); i++)
	{
		std::pair<char, float> tempPair = { allTypes[i], 1 };
		typesAndWeights.push_back(tempPair);
	}

	gridPos = gPos;
	worldPos = glm::vec2{ gPos.x * tileSize.x, gPos.x * tileSize.x };
	index = intIndex;
}

void Tile::Reset()
{
	typesAndWeights.clear();
	for (int i = 0; i < allTypes.size(); i++)
	{
		std::pair<char, float> tempPair = { allTypes[i], 1 };
		typesAndWeights.push_back(tempPair);
	}
	walkable = true;
	type = '0';
}

void Tile::UpdateTypeandWeight(char c, float weightChange)
{
	auto it = std::find_if(typesAndWeights.begin(), typesAndWeights.end(), [c](const auto& p) {
		return p.first == c;
		});

	if (it != typesAndWeights.end())
	{
		typesAndWeights.at(it - typesAndWeights.begin()).second += weightChange;
	}
}
