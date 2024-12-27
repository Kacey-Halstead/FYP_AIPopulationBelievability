#include "Tile.h"
#include "iostream"

using namespace std;
Tile::Tile(vector<char> allTypes)
{
	availableTypes = allTypes;
	allAvTypes = allTypes;
}

Tile::Tile()
{
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

SDL_Point operator-(const SDL_Point a, const SDL_Point b)
{
	SDL_Point p;
	p.x = a.x - b.x;
	p.y = a.y - b.y;
	return p;
}

bool operator==(const SDL_Point a, const SDL_Point b)
{
	if (a.x == b.x && a.y == b.y)
	{
		return true;
	}
	return false;
}

SDL_Point operator +(const SDL_Point a, const SDL_Point b)
{
	return { (a.x + b.x), (a.y + b.y) };
}