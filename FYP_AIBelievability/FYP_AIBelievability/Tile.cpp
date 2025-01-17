#include "Tile.h"
#include "iostream"

using namespace std;
Tile::Tile(std::array<char, numberOfTypes> allTypes)
{
	for (int i = 0; i < allTypes.size(); i++)
	{
		std::pair<char, float> tempPair = { allTypes[i], 1 };
		typesAndWeights.push_back(tempPair);
	}
	allAvTypes = allTypes;
}

Tile::Tile()
{
}

void Tile::SetType(char newType)
{
	type = newType;
	if (type == 'S') walkable = false;
}

void Tile::Print()
{
	cout << type;
}

void Tile::Reset()
{
	typesAndWeights.clear();
	for (int i = 0; i < allAvTypes.size(); i++)
	{
		std::pair<char, float> tempPair = { allAvTypes[i], 1 };
		typesAndWeights.push_back(tempPair);
	}
	walkable = true;
	type = '0';
}