#include "Tile.h"
#include "iostream"

using namespace std;
Tile::Tile(vector<char> allTypes)
{
	availableTypes = allTypes;
	allAvTypes = allTypes;
}

void Tile::SetType(char newType)
{
	type = newType;
}

void Tile::Print()
{

	cout << type;
}

void Tile::Reset()
{
	availableTypes = allAvTypes;
}