#pragma once
#include <vector>

using namespace std;
class Tile
{
public:
	char type = '0';
	vector<char> availableTypes;
	vector<char> allAvTypes;
	vector<int> pos;

	Tile(vector<char> allTypes);
	~Tile();
	void SetType(char newType);
	void Print();
	void Reset();
private:

};

