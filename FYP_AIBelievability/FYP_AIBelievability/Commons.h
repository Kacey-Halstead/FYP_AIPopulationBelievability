#pragma once
#include <string>
#include <array>


struct Trait
{
	std::string traitName;
	int traitEffect;
	int OCEANEffect;
};

struct Needs
{
	float hungerVal = 100;
	float thirstVal = 100;
};

constexpr int gridSizeX = 10;
constexpr int gridSizeY = 10;
constexpr std::array<char, 3> allTypes = { 'L', 'C', 'S' };
constexpr int numberOfTypes = allTypes.size();

