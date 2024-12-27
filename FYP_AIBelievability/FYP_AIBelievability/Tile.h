#pragma once
#include <vector>
#include <SDL_rect.h>

using namespace std;
class Tile
{
public:
	char type = '0';
	vector<char> availableTypes;
	vector<char> allAvTypes;
	SDL_Point pos;

	Tile();
	Tile(vector<char> allTypes);
	~Tile();
	void SetType(char newType);
	void Print();
	void Reset();



private:

};

SDL_Point operator-(SDL_Point a, SDL_Point b)
{
	SDL_Point p;
	p.x = a.x - b.x;
	p.y = a.y - b.y;
	return p;
}

