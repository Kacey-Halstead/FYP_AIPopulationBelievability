#pragma once
#include <string>
#include <array>
#include <glm/glm.hpp>

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



constexpr int gridSizeX = 30;
constexpr int gridSizeY = 30;
constexpr std::array<char, 3> allTypes = { 'L', 'C', 'S' };
constexpr int numberOfTypes = allTypes.size();

bool ComparePositions(glm::vec2 a, glm::vec2 b);