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

constexpr int gridSizeX = 50;
constexpr int gridSizeY = 50;
constexpr std::array<char, 3> allTypes = { 'L', 'C', 'S' };
constexpr int numberOfTypes = allTypes.size();


