#pragma once
#include <vector>
#include <array>
#include <SDL_rect.h>

using namespace std;

SDL_Point operator-(const SDL_Point a, const SDL_Point b);

bool operator==(const SDL_Point a, const SDL_Point b);

SDL_Point operator +(const SDL_Point a, const SDL_Point b);

class Tile
{
public:
	char type = '0';
	vector<char> availableTypes; //currently available types
	vector<char> allAvTypes; //all available types possible
	SDL_Point pos;
	bool walkable = true;
	bool isInPath = false;
	int index = 0;

	Tile(vector<char> allTypes);
	Tile();
	void SetType(char newType);
	void Print();
	void Reset();

	friend bool operator ==(const Tile a, const Tile b)
	{
		return a.pos == b.pos;
	}
};




