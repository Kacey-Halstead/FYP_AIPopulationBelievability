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
	std::vector<std::pair<char, float>> typesAndWeights;
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




